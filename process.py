#!/usr/bin/env python

import os
import argparse
import cv2
import numpy as np
import matplotlib.pyplot as plt


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("name", nargs="*")
  parser.add_argument("--viz", dest="viz", action="store_true")
  parser.set_defaults(viz=False)
  args = parser.parse_args()

  for name in args.name:
    im = cv2.imread(name, -1).astype(np.float32)

    print(im.max())

    if len(im) < 3:
      im = np.dstack([im]*3)
    chans = 3
    bp = []
    wp = []
    im += 1

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.title("raw")
        plt.xlim([0, 2**16-1])
      plt.savefig("0_raw.pdf")
      plt.clf()

    for c in range(chans):
      mini = np.percentile(im[..., c], 1)
      mini = im[..., c].min()
      im[..., c] -= mini

    for c in range(chans):
      maxi = np.percentile(im[..., c], 99)
      maxi = im[..., c].max()
      im[..., c] /= maxi

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.xlim([0, 1])
        plt.title("levels")
      plt.savefig("1_levels.pdf")
      plt.clf()

    eps = 5e-2
    valid = im >= eps

    do_log = True
    if do_log:
      print("log processing")
      im = np.log2(np.maximum(im, 0) + eps)
      leps = np.log2(eps)

      print(leps)

      if args.viz:
        for c in range(chans):
          plt.subplot(3, 1, 1+c)
          plt.hist(np.ravel(im[..., c]), bins=100)
          plt.title("log")
          plt.xlim([leps, 0])
        plt.savefig("2_log.pdf")
        plt.clf()

      im = - im + leps

      if args.viz:
        for c in range(chans):
          plt.subplot(3, 1, 1+c)
          plt.hist(np.ravel(im[..., c]), bins=100)
          plt.title("inv_log")
          plt.xlim([leps, 0])
        plt.savefig("3_inv_log.pdf")
        plt.clf()

      im = np.power(2, im)
    else:
      im = 1.0 - im

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.title("inv_linear")
        plt.xlim([0, 1])
      plt.savefig("4_inv_linear.pdf")
      plt.clf()

    im -= im.min()

    # White balance
    for c in range(chans):
      chan_data = im[..., c]
      val_data = valid[..., c]
      maxi = im[..., c].max()
      # maxi = chan_data[val_data].max()
      # maxi = np.percentile(chan_data[val_data], 99)
      im[..., c] /= maxi

    print(im.min())
    print(im.max())
    im = np.clip(im, 0, 1)
    # gamma
    # im = np.power(im, 1.0/2.2)

    if args.viz:
      for c in range(chans):
        plt.subplot(3, 1, 1+c)
        plt.hist(np.ravel(im[..., c]), bins=100)
        plt.title("white_balanced")
        plt.xlim([0, 1])
      plt.savefig("5_white_balanced.pdf")
      plt.clf()

    im = (im*(2**16-1)).astype(np.uint16)

    new_name = os.path.splitext(name)[0] + "_processed.tiff"
    print(new_name)
    cv2.imwrite(new_name, im)

if __name__ == "__main__":
  main()

