"""
symlinks.py
Creates a set of symbolic links in the default sketchbook directory for
all Arduino libraries in this repository. The purpose is to make it easy to
install a set of Arduino libraries.
symlinks.py usage:
    python symlinks.py --install  # Creates or refreshes links to the libraries
    python symlinks.py --remove   # Removes links to the libraries
Authors:
Will Dickson will@iorodeo.com
Peter Polidoro polidorop@janelia.hhmi.org
Modified by:
Jonathan Grizou jonathan.grizou@glasgow.ac.uk
Graham Keenan 1105045k@student.gla.ac.uk
"""
import sys
import os
import argparse
import platform

USERDIR = os.path.expanduser('~')
if platform.system() in ['Linux', 'Darwin']:
    LIBDIR = os.path.join(USERDIR, 'sketchbook', 'libraries')
elif platform.system() == 'Windows':
	LIBDIR = os.path.join(USERDIR, 'Documents', 'Arduino', 'libraries')
    #Symlinks for windows
	def symlink_ms(source, link_name):
		import ctypes
		csl = ctypes.windll.kernel32.CreateSymbolicLinkW
		csl.argtypes = (ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint32)
		csl.restype = ctypes.c_ubyte
		flags = 1 if os.path.isdir(source) else 0
		try:
		    if csl(link_name, source.replace('/', '\\'), flags) == 0:
		        raise ctypes.WinError()
		except:
		    pass


def create_symlinks_windows():
	if not os.path.isdir(LIBDIR):
		print ('Libraries directory does not exist - creating...')
		os.makedirs(LIBDIR)

	src_paths, dst_paths = get_paths()
	for src, dst in zip(src_paths, dst_paths):
		if os.path.exists(dst):
			if not isLink(dst):
				print ('{0} exists and in not a symbolic link - not overwriting'.format(dst))
				continue
			else:
				print ('Unlinking {0}'.format(dst))
				os.unlink(dst)

		print('Creating new symbolic link {0}'.format(dst))
		symlink_ms(src, dst)

def create_symlinks_unix():

    # Create library directory if it doesn't exist
    if not os.path.isdir(LIBDIR):
        print('libraries directory does not exist - creating')
        os.makedirs(LIBDIR)

    # Create symbolic links
    src_paths, dst_paths = get_paths()
    for src, dst in zip(src_paths, dst_paths):
        if os.path.exists(dst):
            if not os.path.islink(dst):
                print('{0} exists and in not a symbolic link - not overwriting'.format(dst))
                continue
            else:
                print('unlinking {0}'.format(dst))
                os.unlink(dst)
        # Create symbolic link
        print('creating new symbolic link {0}'.format(dst))
        os.symlink(src, dst)


def remove_symlinks():

    if not os.path.isdir(LIBDIR):
        return

    # Remove symbolic links
    src_paths, dst_paths = get_paths()
    for dst in dst_paths:
        if os.path.islink(dst):
            print('removing symbolic link {0}'.format(dst))
            os.unlink(dst)


def get_paths():
    """
    Get source and destination paths for symbolic links
    """
    curdir = os.path.abspath(os.path.curdir)
    dir_list = os.listdir(curdir)
    src_paths = []
    dst_paths = []
    for item in dir_list:
        if os.path.isdir(item):
            if (item == '.hg') or (item == '.git'):
                continue
            src = os.path.join(curdir, item)
            dst = os.path.join(LIBDIR, item)
            src_paths.append(src)
            dst_paths.append(dst)
    return src_paths, dst_paths

#For Windows only
def isLink(path):
	if os.path.exists(path):
		if os.path.isdir(path):
			FILE_ATTRIBUTE_REPARSE_POINT = 0x4000
			attributes = ctypes.windll.kernel32.GetFileAttributesW(unicode(path))
			return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) > 0
		else:
			command = ['dir', path]
			try:
				with open(os.devnull, 'w') as NULL_FILE:
					o0 = check_output(command, stderr=NULL_FILE, shell=True)
			except CalledProcessError as e:
				print e.check_output
				return False
			o1 = [s.strip() for s in o0.split('\n')]
			if len(o1) < 6:
				return False
			else:
				return 'SYMLINK' in o1[5]
	else:
		return False


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Arduino Libraries Symlinks')
    parser.add_argument('-i', '--install',
                        help='Install all of the Arduino libraries in this repository to the default sketchbook directory as a set of symbolic links.',
                        action='store_true')
    parser.add_argument('-r', '--remove',
                        help='Remove all of the Arduino library symbolic links from the default sketchbook directory.',
                        action='store_true')

    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    if args.install:
    	if platform.system() in ['Linux', 'Darwin']:
        	create_symlinks_unix()
        elif platform.system() == 'Windows':
        	create_symlinks_windows()
    elif args.remove:
        remove_symlinks()
