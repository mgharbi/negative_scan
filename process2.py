#!/usr/bin/env python

import os
import subprocess
import argparse
import cv2
import numpy as np
import matplotlib.pyplot as plt

# dcraw command
# dcraw -T -6  -W +M -H 0 -w -g 1 1 -v -o 6 <fname>
# tiff, 16 bits, fixed white level, no highlight clip, linear curve, verbose, ACES output color space


# BGR alphas
LOG_SLOPES = {
    "portra400": [1.6338, 1.679540, 1.7317],
    "ektar": [567.27/379.71, 464.54/264.01, 455.43/254.72],
}

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("name", nargs="*")
  parser.add_argument("--viz", dest="viz", action="store_true")
  parser.add_argument("--gamma", nargs=3, default=[1, 1, 1], type=float)
  parser.add_argument("--gamma_preset", choices=LOG_SLOPES.keys())
  parser.add_argument("--scale", nargs=3, default=[1, 1, 1], type=float)
  parser.set_defaults(viz=False)
  args = parser.parse_args()


  if args.gamma_preset is not None:
    print("Using gamma preset", args.gamma_preset)
    args.gamma = LOG_SLOPES[args.gamma_preset]

  for name in args.name:
    ext = os.path.splitext(name)[-1]
    if ext not in [".tif", "tiff"]:
      print ("Running dcraw on", name)
      cmd = ["dcraw", "-T", "-6", "-W", "-H", "0", "-w", "-g", "1", "1", "-v", "-o", "6", name] 
      ret = subprocess.call(cmd)
      print("  ret", ret)

      name = os.path.splitext(name)[0] + ".tiff"

    im = cv2.imread(name, -1).astype(np.float32)

    print("adding offset")
    if len(im) < 3:
      im = np.dstack([im]*3)
    chans = 3
    im += 1

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.title("raw")
        plt.xlim([0, 2**16-1])
      plt.savefig("0_raw.pdf")
      plt.clf()

    # White level of negative
    print("white level")
    for c in range(chans):
      maxi = np.percentile(im[..., c], 99.99)
      print("WL", c, maxi)
      im[..., c] /= maxi

    im = np.minimum(im, 1)

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.xlim([0, 1])
        plt.title("levels")
      plt.savefig("1_levels.pdf")
      plt.clf()

    # invert
    print("Gamma and log-inversion", args.gamma)
    for c in range(chans):
      im[..., c] = -(1.0/args.gamma[c])*np.log10(im[..., c])
      # im[..., c] /= np.percentile(im[..., c], 99)
      # im[..., c] *= 3

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=1000)
        plt.xlim([0, 10])
        plt.title("invert")
      plt.savefig("2_log.pdf")
      plt.clf()

    # de-log
    print("de-log")
    for c in range(chans):
      # not clear what the shift means
      im[..., c] = (np.power(10, im[..., c]))*args.scale[c]
      # im[..., c] = (np.exp(im[..., c]) - 1.0)*args.scale[c]
      mini = im[..., c].min()
      im -= im.min()

    # Maximize range

    # im = np.clip(im, 0, 1)

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.xlim([0, 5])
        plt.title("invert")
      plt.savefig("3_exp.pdf")
      plt.clf()


    im = (im*(2**16-1)).astype(np.uint16)

    new_name = os.path.splitext(name)[0] + "_processed.tiff"
    print(new_name)
    cv2.imwrite(new_name, im)

if __name__ == "__main__":
  main()

