#!/usr/bin/env python
import argparse
import shutil
from pathlib import Path
from import_simplicity_sdk import paths

lib_paths = [
  "platform/radio/rail_lib/autogen/librail_release/librail_*_gcc_release.a",
  "protocol/bluetooth/bgstack/ll/lib/libbluetooth_controller_*_gcc_release.a",
  "protocol/bluetooth/bgcommon/lib/build/gcc/**/bgcommon/release/libbgcommon.a",
]

def copy_existing_files(src: Path, dst: Path, paths: list[str], verbose=False) -> None:
  for path in paths:
    for f in src.glob(path):
      destfile = dst / f.relative_to(src)
      if not destfile.exists():
        if verbose is True:
          print(f"Skipping {f}")
        continue

      if verbose is True:
        print(f"Copy {f} to {destfile}")
      shutil.copy(f, destfile)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--sdk", "-s", type=Path, required=True, help="Path to the local Simplicity SDK")
  headers_libs = parser.add_mutually_exclusive_group()
  headers_libs.add_argument("--headers-only", "-e", action="store_true")
  headers_libs.add_argument("--libs-only", "-l", action="store_true")
  args = parser.parse_args()

  dst = (Path(__file__).parent.parent / "simplicity_sdk").resolve()

  if args.sdk is not None:
    src = args.sdk.resolve(strict=True)

    if args.libs_only is False:
      print(f"Copy SDK headers from {src}")
      copy_existing_files(src, dst, paths, verbose=True)

    if args.headers_only is False:
      print(f"Copy SDK libraries to local blobs")
      blob_dst = (Path(__file__).parent.parent / "zephyr" / "blobs" / "simplicity_sdk").resolve()
      copy_existing_files(src, blob_dst, lib_paths, verbose=True)

    print("Done")
    print("""
    ********************************************
    *** DO NOT COMMIT COPIED FILES RANDOMLY! ***
    ********************************************\n""")
    print(" This script does not parse the slcc trees for the files that are actually intended to be published!")
  else:
    print("No SDK to import from")

