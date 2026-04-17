import argparse

parser = argparse.ArgumentParser(prog="cryptalk" ,description="End to end cryptographic chat")
subparser = parser.add_subparsers()

newParser = subparser.add_parser("new")

openParser = subparser.add_parser("open")

def main():
    args = parser.parse_args()

if __name__ == "__main__":
    main()