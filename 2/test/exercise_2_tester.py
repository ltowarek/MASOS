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

    logging.debug('{} content:\n{}'.format(file_path, output))
    return output


def execute_command(command, arguments, timeout):
    output = None

    try:
        # Input parameter is used because if all arguments are passed in command then MemoryError raises
        output = subprocess.check_output(command, input=arguments, shell=True, stderr=subprocess.STDOUT,
                                         universal_newlines=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        logging.error('Command timed out!')
    except subprocess.CalledProcessError as error:
        logging.error('Failed to execute command!')
        logging.error('Output: {}'.format(error.output))

    logging.debug('Output:\n{}'.format(output))

    return output


def is_output_valid(actual_output, expected_output):
    if not actual_output or len(actual_output) < 512:
        logging.info('Actual output:\n{}'.format(actual_output))
    else:
        logging.debug('Actual output:\n{}'.format(actual_output))

    if not expected_output or len(expected_output) < 512:
        logging.info('Expected output:\n{}'.format(expected_output))
    else:
        logging.debug('Expected output:\n{}'.format(expected_output))
    is_valid = True if actual_output == expected_output else False
    if actual_output and expected_output:
        splitted_actual_output = actual_output.split('\n')
        splitted_expected_output = expected_output.split('\n')
        if splitted_actual_output[0] == splitted_expected_output[0]:
            logging.info('CHI output is equal')
        if splitted_actual_output[1] == splitted_expected_output[1]:
            logging.info('K_PLUS output is equal')
        if splitted_actual_output[2] == splitted_expected_output[2]:
            logging.info('K_MINUX output is equal')
    logging.debug('Is valid: {}'.format(is_valid))
    return is_valid


def run(args):
    logging.info('Testing {} - {}'.format(args.input_file, args.output_file))
    arguments = read_file(args.input_file)
    actual_output = execute_command(args.command, arguments, args.timeout)
    expected_output = read_file(args.output_file)
    is_valid = is_output_valid(actual_output, expected_output)
    logging.info('TEST PASSED\n') if is_valid else logging.info('TEST FAILED\n')
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
