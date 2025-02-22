# evaluation.py

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix, accuracy_score
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import json

# Load the trained model
model = load_model('watch_classifier.h5')

# Define image size and batch size
IMG_HEIGHT, IMG_WIDTH = 224, 224
BATCH_SIZE = 32

# Data generators
validation_datagen = ImageDataGenerator(rescale=1./255)

# Load validation data
validation_generator = validation_datagen.flow_from_directory(
    'data/validation',
    target_size=(IMG_HEIGHT, IMG_WIDTH),
    batch_size=BATCH_SIZE,
    class_mode='categorical',
    shuffle=False  # Important to keep the order of images
)

# Generate predictions
validation_generator.reset()
y_true = validation_generator.classes
y_pred = model.predict(validation_generator)
y_pred_classes = np.argmax(y_pred, axis=1)

# Calculate accuracy
accuracy = accuracy_score(y_true, y_pred_classes)
print(f"Validation Accuracy: {accuracy:.2f}")

# Plot confusion matrix
def plot_confusion_matrix(y_true, y_pred, class_labels):
    cm = confusion_matrix(y_true, y_pred)
    plt.figure(figsize=(8, 6))
    sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', 
                xticklabels=class_labels, yticklabels=class_labels)
    plt.title('Confusion Matrix')
    plt.xlabel('Predicted Label')
    plt.ylabel('True Label')
    plt.show()

# Plot accuracy curves
def plot_accuracy_curves(history):
    train_acc = history['accuracy']
    val_acc = history['val_accuracy']
    plt.figure(figsize=(10, 5))
    plt.plot(train_acc, label='Training Accuracy')
    plt.plot(val_acc, label='Validation Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.title('Training and Validation Accuracy')
    plt.legend()
    plt.show()

# Plot confusion matrix
class_labels = ['analog', 'digital', 'smart']
plot_confusion_matrix(y_true, y_pred_classes, class_labels)

# Assuming you have the training history saved from the training script
# Load the training history
with open('training_history.json', 'r') as f:
    history_data = json.load(f)

# Plot accuracy curves
plot_accuracy_curves(history_data)