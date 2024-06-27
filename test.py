import os
import subprocess
import pytest

def pytest_generate_tests(metafunc):
    test_dir = 'test'
    test_cases = []

    for filename in os.listdir(test_dir):
        if filename.endswith('.in'):
            test_name = filename[:-3]
            input_file = os.path.join(test_dir, filename)
            expected_output_file = os.path.join(test_dir, f'{test_name}.out')
            test_cases.append((test_name, input_file, expected_output_file))

    metafunc.parametrize("test_name,input_file,expected_output_file", test_cases)

def run_polyproject(input_file):
    with open(input_file, 'r') as infile:
        input_data = infile.read()
    result = subprocess.run(['./poly'], input=input_data, text=True, capture_output=True)
    return result.stdout.strip()

def test_polyproject(test_name, input_file, expected_output_file):
    output = run_polyproject(input_file)
    with open(expected_output_file, 'r') as outfile:
        expected_output = outfile.read().strip()

    assert output == expected_output, f'Test {test_name} FAILED\nExpected:\n{expected_output}\nGot:\n{output}'
