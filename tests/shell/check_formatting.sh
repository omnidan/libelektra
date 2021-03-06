@INCLUDE_COMMON@

echo
echo ELEKTRA CHECK FORMATTING
echo

command -v git >/dev/null 2>&1 || { echo "git command needed for this test, aborting" >&2; exit 0; }
command -v clang-format-5.0 >/dev/null 2>&1 || command -v clang-format >/dev/null 2>&1 || { echo "clang-format command needed for this test, aborting" >&2; exit 0; }

cd "@CMAKE_SOURCE_DIR@"

if ! git diff --exit-code
then
	echo "Seems like source is already modified, aborting test" >&2
	exit 0
fi

scripts/reformat-source

if which sponge > /dev/null && which cmake-format > /dev/null
then
	scripts/reformat-cmake
else
	echo 'Warning: Since either `sponge` or `cmake-format` are not available I will not check the formatting of the CMake code.'
fi

git diff --exit-code
succeed_if "Please commit the reformatting changes before pushing"

end_script
