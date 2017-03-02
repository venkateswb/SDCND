
# coding: utf-8

# In[1]:

import cv2
import numpy as np
import pandas as pd
import random
import os
import csv
from sklearn.utils import shuffle
from keras.models import Sequential
from keras.layers import Convolution2D, Cropping2D, Flatten, Dropout, Dense, Lambda, MaxPooling2D, Activation
from keras.optimizers import RMSprop,Adam
from keras.callbacks import ModelCheckpoint, Callback, EarlyStopping
from sklearn.model_selection import train_test_split


# In[2]:

def cloning_model():
    
    model = Sequential()

    # Source:  https://images.nvidia.com/content/tegra/automotive/images/2016/solutions/pdf/end-to-end-dl-using-px.pdf
    model = Sequential()

    model.add(Lambda(lambda x: x / 127.5 - 1.0, input_shape=(64, 64, 3)))

    # starts with five convolutional and maxpooling layers
    model.add(Convolution2D(24, 5, 5, border_mode='same', subsample=(2, 2)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(1, 1)))
    
    # 2nd layer with five convolutional and maxpooling layers
    model.add(Convolution2D(36, 5, 5, border_mode='same', subsample=(2, 2)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(1, 1)))

    # 3rd layer with five convolutional and maxpooling layers
    model.add(Convolution2D(48, 5, 5, border_mode='same', subsample=(2, 2)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(1, 1)))

    # 4th layer with five convolutional and maxpooling layers
    model.add(Convolution2D(64, 3, 3, border_mode='same', subsample=(1, 1)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(1, 1)))

    # 5th layer with five convolutional and maxpooling layers
    model.add(Convolution2D(64, 3, 3, border_mode='same', subsample=(1, 1)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(1, 1)))

    #Faltten the output
    model.add(Flatten())

    # Next, five fully connected layers with Dense 1164
    model.add(Dense(1164))
    model.add(Activation('relu'))
    
    # Fully connected layers with Dense 100
    model.add(Dense(100))
    model.add(Activation('relu'))

    # Fully connected layers with Dense 50
    model.add(Dense(50))
    model.add(Activation('relu'))
    
    # Dropout function 
    model.add(Dropout(0.5))
    model.add(Activation('relu'))
    
    # Fully connected layers with Dense 10
    model.add(Dense(10))
    model.add(Activation('relu'))
    
    # Fully connected layers with Dense 1
    model.add(Dense(1))

    model.compile(optimizer=Adam(1e-4), loss="mse", )

    return model

model =  cloning_model()


# In[3]:

#Crop and Re-Size the Image
def img_resize(img):
    ratio = img[64:130,:,:]
    res_image = cv2.resize(ratio,(64,64),interpolation=cv2.INTER_AREA)
    return res_image


# In[4]:

# Flip the image and measure the steering angle for Fliped Image
def image_flip(image,steering):
    if random.randrange(2) == 1: 
        return cv2.flip(image,1), -1 * steering
    else: 
        return image,steering


# In[5]:

# Generator function is used to sample the batches
def generator(samples, batch_size):
    num_samples = len(samples)
    while 1: # Loop forever so the generator never terminates
        shuffle(samples)
        for offset in range(0, num_samples, batch_size):
            batch_samples = samples[offset:offset+batch_size]
            images = []
            angles = []
            for batch_sample in batch_samples:
                
                # Load the images and angles i.e., Center, Left and Right
                rnd = np.random.randint(0, 3)
                if rnd == 0 : 
                    load_image = './data/IMG/'+batch_sample[0].split('/')[-1]
                    load_angle = float(batch_sample[3])
                
                elif rnd == 1:
                    load_image = './data/IMG/'+batch_sample[1].split('/')[-1]
                    load_angle = float(batch_sample[3]) + 0.25
                                
                else:
                    load_image = './data/IMG/'+batch_sample[2].split('/')[-1]
                    load_angle = float(batch_sample[3]) - 0.25

                # Crop and Resize the image    
                image = img_resize(cv2.imread(load_image))
                #Flip the images and measure the angle
                image, angle = image_flip(image, load_angle)
                
                #append all Calculated images
                images.append(image)
                angles.append(angle)
                
            # trim image to only see section with road
            X_train = np.array(images)
            y_train = np.array(angles)
            yield shuffle(X_train, y_train)


# In[6]:

#Main 
if __name__ == '__main__':
    samples = []
    with open('./data/driving_log.csv') as csvfile:
        reader = csv.reader(csvfile)
        for line in reader:
            samples.append(line)

    train_samples, validation_samples = train_test_split(samples, test_size=0.2)

    # compile and train the model using the generator function
    train_generator = generator(train_samples, batch_size=128)
    validation_generator = generator(validation_samples, batch_size=64)

    #Execute the model provide training and validation set
    model.fit_generator(train_generator, validation_data=validation_generator,
                        samples_per_epoch=20096, nb_epoch=9, nb_val_samples=6000)

    print("Saving model weights and configuration file.")
    model.save('model.h5')  # always save your weights after training 


# In[ ]:



