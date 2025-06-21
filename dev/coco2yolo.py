import logging


logging.getLogger().setLevel(logging.CRITICAL)


from pylabel import importer

import os 
import zipfile

#Specify path to the coco.json file
path_to_annotations = "/home/alexey/Insync/afokinkrd@gmail.com/Google Drive/NN/coins/annotations/instances_default.json"
#Specify the path to the images (if they are in a different folder than the annotations)
path_to_images = ""

#Import the dataset into the pylable schema 
dataset = importer.ImportCoco(path_to_annotations, path_to_images=path_to_images, name="Segmentation")
print(dataset.analyze.num_images)


#dataset.path_to_annotations = "data/yolo"
#dataset.export.ExportToYoloV5(segmentation=True)


