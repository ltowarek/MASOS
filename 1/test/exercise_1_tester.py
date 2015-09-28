#!/usr/bin/env python3.4

import argparse
import logging
import subprocess
import sys


def process_command_line():
    parser = argparse.ArgumentParser()

    parser.add_argument('command')
    parser.add_argument('--timeout', type=int, default=60, help='commands timeout in seconds (default: %(default)d)')
    parser.add_argument('--debug', action='store_true', help='enable debug mode')

    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    logging.debug('Arguments: {}'.format(args))

    return args


def run(args):
    output = None

    try:
        output = subprocess.check_output(args.command, shell=True, stderr=subprocess.STDOUT, universal_newlines=True,
                                         timeout=args.timeout)
    except subprocess.CalledProcessError as error:
        logging.error('Error occurred!')
        output = error.output
    except subprocess.TimeoutExpired:
        logging.error('Command timed out!')

    logging.info("Output: {0}".format(output))


def main():
    try:
        args = process_command_line()
        run(args)
    except:
        logging.exception('Exception occurred!')
        return -1

    return 0


if __name__ == '__main__':
    status = main()
    sys.exit(status)