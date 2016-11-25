from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import scipy.ndimage as ndimg


def fft_arr(arr):
    plt.gray()
    plt.figure(1)
    plt.subplot(2, 2, 1)
    plt.imshow(arr)
    fft = np.fft.fft2(arr)
    plt.subplot(2, 2, 2)
    plt.imshow(np.abs(fft))
    plt.subplot(2, 2, 3)
    plt.imshow(np.abs(np.fft.fftshift(fft)))
    plt.subplot(2, 2, 4)
    with np.errstate(divide='ignore'):
        plt.imshow(np.log(np.abs(np.fft.fftshift(fft))))
    plt.show()


def fft_img(img):
    img = img.convert('L')
    arr = np.asarray(img.getdata()).reshape(img.size)
    fft_arr(arr)


def exp1():
    n = range(128)
    x = np.sin(np.divide(np.multiply(np.pi, n), 8))
    x2 = []
    for i in range(128):
        x2.append(x)
    fft_arr(x2)


def exp2():
    x = np.zeros((512, 512))
    for i in range(20):
        for j in range(40):
            x[256 - 10 + i, 256 - 20 + j] = 255;
    fft_arr(x)
    x = np.zeros((512, 512))
    for i in range(150):
        for j in range(20):
            x[256 - 75 + i, 256 - 10 + j] = 255;
    x = ndimg.rotate(x, 30)
    fft_arr(x)
    x = np.zeros((512, 512))
    for i in range(150):
        for j in range(20):
            x[256 - 200 + i, 256 - 30 + j] = 255;
    x = ndimg.rotate(x, 30)
    fft_arr(x)


def exp3():
    fft_img(Image.open('profile.jpg'))


if __name__ == '__main__':
    exp1()
    exp2()
    exp3()
