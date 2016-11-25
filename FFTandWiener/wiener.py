from PIL import Image
import matplotlib.pyplot as plt
import numpy as np


def make_motion_psf(length, size):
    sx, sy = size
    psf = np.zeros((sy, sx))
    psf[int(sy / 2):int(sy / 2 + 1), int(sx / 2 - length / 2):int(sx / 2 + length / 2)] = 1
    return psf / psf.sum()


def convolve(input, psf, epsilon):
    input_fft = np.fft.fft2(input)
    psf_fft = np.fft.fft2(psf)
    convolved = np.fft.ifft2(input_fft * psf_fft)
    convolved = np.abs(np.fft.fftshift(convolved))
    return convolved


def deconvolve(input, psf, epsilon):
    input_fft = np.fft.fft2(input)
    psf_fft = np.fft.fft2(psf) + epsilon
    res = np.fft.ifft2(input_fft / psf_fft)
    res = np.abs(np.fft.fftshift(res))
    return res


img = Image.open('profile.jpg').convert('L')
plt.figure(1)
plt.gray()
data = np.asarray(img.getdata()).reshape(img.size)
psf = make_motion_psf(30, data.shape)
blurred = np.abs(convolve(data, psf, 0))
plt.subplot(221)
plt.imshow(blurred)
res = deconvolve(blurred, psf, 1e-6)
plt.subplot(222)
plt.imshow(res)
blurred += 0.1 * blurred.std() * np.random.standard_normal(blurred.shape)
plt.subplot(223)
plt.imshow(blurred)
res = deconvolve(blurred, psf, 0.09)
plt.subplot(224)
plt.imshow(res)
plt.show()
