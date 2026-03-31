"""
Reads RPY comparison data from the Pico's USB serial port and saves to CSV.

The firmware outputs one CSV line per sample (~10 Hz) when compare_rpy=1:
  timestamp_us,native_r,native_p,native_y,quat_r,quat_p,quat_y

Usage:
    python3 rpy_logger.py
    python3 rpy_logger.py --port /dev/ttyACM1 --duration 60 --out my_data.csv
"""

import argparse
import csv
import time
import serial
import serial.tools.list_ports

HEADER = ["timestamp_us", "native_r", "native_p", "native_y", "quat_r", "quat_p", "quat_y"]


def find_pico_port():
    for p in serial.tools.list_ports.comports():
        if "ACM" in p.device or "usbmodem" in p.device:
            return p.device
    return "/dev/ttyACM0"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default=None, help="Serial port (auto-detected if omitted)")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--duration", type=float, default=60.0, help="Logging duration in seconds")
    parser.add_argument("--out", default="rpy_data.csv", help="Output CSV filename")
    args = parser.parse_args()

    port = args.port or find_pico_port()
    print(f"Opening {port} at {args.baud} baud")
    print(f"Logging for {args.duration}s -> {args.out}")
    print("Press Ctrl+C to stop early\n")

    with serial.Serial(port, args.baud, timeout=1) as ser, \
         open(args.out, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(HEADER)
        start = time.time()
        count = 0
        while time.time() - start < args.duration:
            try:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line:
                    continue
                parts = line.split(",")
                if len(parts) != 7:
                    continue  # skip boot messages and other non-data lines
                float(parts[1])  # validate it's a data row, not a text message
                writer.writerow(parts)
                f.flush()
                count += 1
                if count % 50 == 0:
                    print(f"  {count} samples ({time.time() - start:.1f}s elapsed)")
            except (ValueError, UnicodeDecodeError):
                continue
            except KeyboardInterrupt:
                break

    print(f"\nDone. {count} samples saved to {args.out}")


if __name__ == "__main__":
    main()
