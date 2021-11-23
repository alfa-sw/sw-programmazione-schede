# Usage

- Update _CHANGELOG.txt.
- Increment version by changing file `__version__` in root projects.

Note: <OUT_DIR> is the output folder; defined as OUT_DIR in make-bdist.

1. Adjust env variables in `make-bdist.sh`
2. `./make-bdist.sh`
3. `./<OUT_DIR>/stage1/make_exe_linux.sh` 
4. copy folder `stage1_win` to windows and execute `make_exe_win.bat`
5. copy content of subfolder `dist` to `stage2`
6. execute `stage2/finalize.sh`

The output zip file is placed in the output folder.
