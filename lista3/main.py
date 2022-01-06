# for loading/processing the images
from keras.preprocessing.image import load_img
from keras.preprocessing.image import img_to_array
from keras.applications.vgg16 import preprocess_input

# models
from keras.models import Model
from keras.datasets import mnist

# clustering and dimension reduction
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from sklearn import metrics
from sklearn.metrics import confusion_matrix
from sklearn.utils import Bunch

# for everything else
import os
import numpy as np
import matplotlib.pyplot as plt
from random import randint
import pandas as pd
import pickle
import seaborn as sns
from PIL import Image
sns.set()


def loadAdnShowData(show):
    (trainX, trainY), (testX, testY) = mnist.load_data()

    if show:
        fig, axs = plt.subplots(3, 3, figsize=(12, 12))
        plt.gray()

        for i, ax in enumerate(axs.flat):
            ax.matshow(trainX[i])
            ax.axis('off')
        fig.show()

    return (trainX, trainY), (testX, testY)


def getClustersLabels(k_means, actual_labels):

    inferred_labels = {}

    for i in range(k_means.n_clusters):

        labels = []
        index = np.where(k_means.labels_ == i)

        labels.append(actual_labels[index])

        if len(labels[0]) == 1:
            counts = np.bincount(labels[0])
        else:
            counts = np.bincount(np.squeeze(labels))

        if np.argmax(counts) in inferred_labels:
            inferred_labels[np.argmax(counts)].append(i)
        else:
            inferred_labels[np.argmax(counts)] = [i]

    return inferred_labels


def getPredictedLabels(labels, cluster_labels):
    predicted_labels = np.zeros(len(labels)).astype(np.uint8)

    for i, cluster in enumerate(labels):
        for key, value in cluster_labels.items():
            if cluster in value:
                predicted_labels[i] = key

    return predicted_labels


def predictFor10():
    kMeans = KMeans(n_clusters=10)
    kMeans.fit(x)

    predicted_labels = getPredictedLabels(kMeans.labels_, getClustersLabels(kMeans, y))
    print(predicted_labels[:10])
    print(y[:10])
    print(f"Accuracy: {metrics.accuracy_score(train_y, predicted_labels)}")


def getCharts():
    clusters = range(7, 13)
    inertia = []
    accuracy = []

    images, labels = getMyData2()
    for k in clusters:
        kMeans = KMeans(n_clusters=k)
        kMeans.fit(x)

        cluster_labels = getClustersLabels(kMeans, y)
        predicted_labels = getPredictedLabels(kMeans.labels_, cluster_labels)

        getConfusionMatrix(train_y, predicted_labels, k)
        getCentroids(kMeans.cluster_centers_, cluster_labels, k)

        # l = []
        # p = []
        # for i in range(len(images)):
        #     test_clusters = kMeans.predict(images[i])
        #     predicted_label = getPredictedLabels(test_clusters, cluster_labels)
        #     l.append(labels[i])
        #     p.append(predicted_label[0])
        #
        # getConfusionMatrix(l, p, k)

        inertia.append(kMeans.inertia_)
        accuracy.append(metrics.accuracy_score(train_y, predicted_labels))

    plt.plot(clusters, inertia)
    plt.show()
    plt.plot(clusters, accuracy)
    plt.show()

    # images, labels = getMyData("numbers", "png")
    # kMeans = KMeans(n_clusters=10)
    # data = Bunch(data=np.array(images))
    # kMeans.fit(data.data)
    #
    # cluster_labels = getClustersLabels(kMeans, labels)
    # predicted_labels = getPredictedLabels(kMeans.labels_, cluster_labels)
    #
    # getConfusionMatrix(labels, predicted_labels, 10)
    # getCentroids(kMeans.cluster_centers_, cluster_labels, 10)
    #
    # images, labels = getMyData("numbers3", "jpeg")
    # kMeans = KMeans(n_clusters=10)
    # data = Bunch(data=np.array(images))
    # kMeans.fit(data.data)

    cluster_labels = getClustersLabels(kMeans, labels)
    predicted_labels = getPredictedLabels(kMeans.labels_, cluster_labels)

    getConfusionMatrix(labels, predicted_labels, 10)
    getCentroids(kMeans.cluster_centers_, cluster_labels, 10)


def getCentroids(centroids, cluster_labels, k):
    images = centroids.reshape(k, 28, 28)
    images *= 255
    images = images.astype(np.uint8)

    rows = k / 5
    if rows % 1 > 0:
        rows += 1
    rows = int(np.floor(rows))

    fig, axs = plt.subplots(rows, 5, figsize=(10, rows * 3))
    plt.gray()

    for i, ax in enumerate(axs.flat):
        if i < k:
            for key, value in cluster_labels.items():
                if i in value:
                    ax.set_title(f"Label: {key}")
            ax.matshow(images[i])
        ax.axis('off')
    fig.show()


def getConfusionMatrix(yy, predicted_labels, k):

    mat = confusion_matrix(yy, predicted_labels, normalize='pred')

    plt.figure(figsize=(15, 8))
    sns.heatmap(mat.T, square=True, annot=True, cbar=False, linewidths=3.5)
    plt.title(f"Clusters: {k}")
    plt.xlabel("correct label")
    plt.ylabel("predicted label")
    plt.show()


def getMyData(folder, type):
    images = []
    labels = []
    for i in range(10):
        for j in range(1, 4):
            img2 = Image.open(f'{folder}/{i}_{j}.{type}').convert("L")
            img = np.resize(img2, (1, 784, 1))
            im2arr = np.asarray(img)
            im2arr = im2arr.reshape(-1)
            im2arr = im2arr.astype(float) / 255
            images.append(im2arr)
            labels.append(i)
    labels = np.asarray(labels)
    return images, labels


def getMyData2():
    images = []
    labels = []
    for i in range(10):
        for j in range(1, 4):
            img2 = Image.open(f'numbers/{i}_{j}.png').convert("L")
            img = np.resize(img2, (1, 784, 1))
            im2arr = np.array(img)
            im2arr = im2arr.reshape(len(im2arr), -1)
            im2arr = im2arr.astype(float) / 255
            images.append(im2arr)
            labels.append(i)
    labels = np.asarray(labels)
    return images, labels


if __name__ == "__main__":
    (train_x, train_y), (test_x, test_y) = loadAdnShowData(False)

    x = train_x.reshape(len(train_x), -1)
    y = train_y

    x = x.astype(float) / 255

    # pca = PCA(n_components=100)
    # pca.fit(x)
    # x = pca.transform(x)

    getCharts()
