best way to profile a script is



python3 -m cProfile -o <profile_output> <script_name>

in order to visualize results 2 options can be used:

- script break down using web-gui (pip install snakeviz)
cli command: snakeviz <profile_output>
- cli breakdown using wrapper script
./print_profile_output.py <profile_output>