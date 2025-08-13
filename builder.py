import subprocess
import argparse
import os
import shutil

if __name__ == '__main__':
	parser = argparse.ArgumentParser(
		prog='Builder',
		description='Use this program to help build the various OrinWatch submodules',
	)

	parser.add_argument('-t', '--target', type=str)
	parser.add_argument('-b', '--build', action='store_true')
	parser.add_argument('-i', '--install', action='store_true')
	parser.add_argument('-c', '--clean', action='store_true')
	parser.add_argument('-d', '--debug', action='store_true')

	args = parser.parse_args()

	targets = ['orinstats', 'orinwatch'] if args.target is None else [args.target]

	debug_dir = os.path.join(os.getcwd(), 'debug')
	release_dir = os.path.join(os.getcwd(), 'release')

	if args.clean:
		if os.path.exists(release_dir):
			shutil.rmtree(release_dir)
		if os.path.exists(debug_dir):
			shutil.rmtree(debug_dir)

	if args.build or args.install:
		if args.debug:
			if not os.path.exists(debug_dir):
				os.mkdir(debug_dir)
			os.chdir(debug_dir)
			subprocess.run(['cmake', '-DCMAKE_BUILD_TYPE=Debug', '..'])
			for target in targets:
				subprocess.run(['make', target])
		else:
			if not os.path.exists(release_dir):
				os.mkdir(release_dir)
			os.chdir(release_dir)
			subprocess.run(['cmake', '..'])
			for target in targets:
				subprocess.run(['make', target])

	if args.install:
		if args.debug:
			os.chdir(debug_dir)
			subprocess.run(['make', 'install'])
		else:
			os.chdir(release_dir)
			subprocess.run(['make', 'install'])
