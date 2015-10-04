#!/usr/bin/env python3.4

import argparse
import logging
import subprocess
import sys


def process_command_line():
    parser = argparse.ArgumentParser()

    parser.add_argument('command')
    parser.add_argument('input_file')
    parser.add_argument('output_file')
    parser.add_argument('--timeout', type=int, default=60, help='commands timeout in seconds (default: %(default)d)')
    parser.add_argument('--debug', action='store_true', help='enable debug mode')

    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    logging.debug('Arguments: {}'.format(args))
    return args


def read_file(file_path):
    output = ''

    try:
        with open(file_path) as file_content:
            output = file_content.read()
    except OSError:
        logging.error('Failed to open {}!'.format(file_path))
        raise

    logging.debug('{} content: {}'.format(file_path, output))
    return output


def execute_command(command, arguments, timeout):
    output = None

    try:
        # Input parameter is used because if all arguments are passed in command then MemoryError raises
        output = subprocess.check_output(command, input=arguments, shell=True, stderr=subprocess.STDOUT,
                                         universal_newlines=True, timeout=timeout)
    except subprocess.CalledProcessError as error:
        logging.error('Failed to execute command!')
        logging.error('Output: {}'.format(error.output))
    except subprocess.TimeoutExpired:
        logging.error('Command timed out!')

    logging.debug('Output: {}'.format(output))

    return output


def is_output_valid(actual_output, expected_output):
    logging.debug('Actual output: {}'.format(actual_output))
    logging.debug('Expected output: {}'.format(expected_output))
    is_valid = True if actual_output == expected_output else False
    logging.debug('Is valid: {}'.format(is_valid))
    return is_valid


def run(args):
    logging.info('Testing {} - {}'.format(args.input_file, args.output_file))
    arguments = read_file(args.input_file)
    actual_output = execute_command(args.command, arguments, args.timeout)
    expected_output = read_file(args.output_file)
    is_valid = is_output_valid(actual_output, expected_output)
    logging.info('TEST PASSED') if is_valid else logging.info('TEST FAILED')
    return is_valid


def main():
    try:
        args = process_command_line()
        test_status = run(args)
    except:
        logging.exception('Exception occurred!')
        return -1

    return test_status


if __name__ == '__main__':
    status = main()
    sys.exit(status)