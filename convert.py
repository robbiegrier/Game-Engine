import subprocess
import argparse
import os

def shell(cmd):
    return subprocess.run(cmd.strip().split(" "), capture_output=True).stdout.decode('utf-8')

def trace(cmd):
    output = shell(cmd)
    print(output)
    return output

def main():
    print("Starting Convert Script")

    parser = argparse.ArgumentParser(
        prog='Convert',
        description='Convert glb models to proto.azul runtime format',
        epilog='By Robbie Grier for the Azul Engine'
    )

    parser.add_argument('-m', '--models', nargs='+', default=[], 
        help="List of models to convert. If none are specified, all *.glb models will be converted."
    )

    parser.add_argument('-v', '--verbose', action='store_true', default=False, 
        help="print verbose output"
    )

    args = parser.parse_args()

    os.chdir("Models")

    shell_cmd = shell
    if args.verbose:
        shell_cmd = trace

    glbs = list(filter(lambda x: x[-4:] == ".glb", os.listdir()))

    glbs_requested = []
    for model in args.models:
        if model[-4:] != ".glb":
            model += ".glb"
        glbs_requested.append(model)
    
    if glbs_requested:
        glbs = list(filter(lambda x: x in glbs_requested, glbs))
    print(f"converting glbs: {glbs}")

    os.chdir("../Data")

    for model in glbs:
        print(f"converting: {model[:-4]}")
        model = model.replace(' ', '%20')
        shell_cmd(f".\..\Debug\Converter.exe {model}")

    print("\nDone!\n")

if __name__ == "__main__":
    main()
