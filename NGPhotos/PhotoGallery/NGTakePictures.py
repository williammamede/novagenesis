import shutil
import threading
import flask
import smtplib
import cv2
import os
import time
import json

from flask_cors import cross_origin
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
from PIL import Image

# Create a Flask app
app = flask.Flask(__name__)

# Create lock for thread-safe app
lock = threading.Lock()

def capture_photo(output_dir):
    print("Capturing photo...")
    print("Opening camera..." + str(cameraIndex))
    # Initialize the webcam
    camera = cv2.VideoCapture(cameraIndex)

    # Wait for a second to let the camera initialize
    time.sleep(cameraDelay)

    # Counter
    counter = 1

    # Filenames to return
    filenames = []

    # Take 3 photos in a 3 second interval
    while counter <= 3:
        print("Taking photo " + str(counter) + "...")
        # read a frame from the webcam and resize it
        ret, frame = camera.read()

        if ret:
            # Generate a filename based on current timestamp
            filename = str(time.time()) + ".jpg"
            filenames.append(filename)
            filepath = os.path.join(output_dir, filename)

            # Save the frame as an image
            cv2.imwrite(filepath, frame)

            print("Photo saved: " + filepath)

            # Increment the counter
            counter += 1
        
        # Wait for 3 seconds
        time.sleep(3)
    
    # Release the camera and close windows
    camera.release()
    cv2.destroyAllWindows()

    print("Photos captured.")
    # Return the filenames
    return filenames
   
def capture_photo_vertically(output_dir):
    # Initialize the webcam with DirectShow backend
    camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)

    # Wait for a second to let the camera initialize
    time.sleep(1)

    # Counter
    counter = 1

    # Filenames to return
    filenames = []

    # Take 3 photos in a 3 second interval
    while counter <= 3:
        # read a frame from the webcam
        ret, frame = camera.read()

        if ret:
            # Generate a filename based on current timestamp
            filename = str(time.time()) + ".jpg"
            filenames.append(filename)
            filepath = os.path.join(output_dir, filename)

            # Save the frame as an image
            cv2.imwrite(filepath, frame)

            print("Photo saved: " + filepath)

            # Open the image with PIL
            img = Image.open(filepath)

            # Get the width and height of the image
            w, h = img.size

            # Calculate the center of the image
            center_x = w // 2
            center_y = h // 2

            # Calculate the coordinates of the cropped region
            left = center_x - 300 # subtract half of the desired width
            right = center_x + 300 # add half of the desired width
            upper = center_y - 540 # subtract half of the desired height
            lower = center_y + 540 # add half of the desired height

            # Crop the image using the calculated coordinates
            cropped_img = img.crop((left, upper, right, lower))

            # Save the cropped image with a new filename
            cropped_filename = str(time.time()) + "_cropped.jpg"
            cropped_filepath = os.path.join(output_dir, cropped_filename)
            cropped_img.save(cropped_filepath)

            print("Cropped photo saved: " + cropped_filepath)

            # Increment the counter
            counter += 1
        
        # Wait for 3 seconds
        time.sleep(3)
    
    # Release the camera and close windows
    camera.release()
    cv2.destroyAllWindows()

    # Return the filenames
    return filenames

def crop_to_nine_sixteen(filename):
    print("Cropping photo to 9:16 aspect ratio in 1080x1920 resolution...")
    # Open the image file
    img = Image.open("./" + filename)

    # Get the width and height of the image
    width, height = img.size

    # Calculate the aspect ratio of the image
    aspect = width / height

    # Define the desired aspect ratio (width / height)
    desired_aspect = 9 / 16

    # Compare the aspect ratios
    if aspect > desired_aspect:
        # If the image is too wide, crop the left and right edges
        new_width = int(desired_aspect * height)
        offset = (width - new_width) // 2
        cropped_region = (offset, 0, width - offset, height)
    elif aspect < desired_aspect:
        # If the image is too tall, crop the top and bottom edges
        new_height = int(width / desired_aspect)
        offset = (height - new_height) // 2
        cropped_region = (0, offset, width, height - offset)
    else:
        # If the image has already the desired aspect ratio, do nothing
        cropped_region = (0, 0, width, height)

    # Crop the image using the calculated region
    cropped_img = img.crop(cropped_region)

    # Generate a new file name based on current timestamp and suffix
    #new_filename = str(time.time()) + "_cropped.jpg"
    #new_filepath = "./" + new_filename

    png_filter = Image.open(filterImageName)

    cropped_img.paste(png_filter, (0, 0), png_filter)

    # Save the cropped image as a new file
    cropped_img.save(filename)

    # Print a message
    print("Cropped photo saved: " + filename)

def send_email(receiverEmail, receiverName):
    print("Sending email to " + receiverEmail + "...")
    mensagem = MIMEMultipart()
    mensagem['From'] = senderEmail
    mensagem['To'] = receiverEmail

    # Email subject from the emailSubject.txt file
    with open('emailSubject.txt', 'r', encoding='utf-8') as f:
        subject = f.read()
    mensagem['Subject'] = subject

    # Email body from the emailBody.html file
    with open('emailBody.html', 'r', encoding='utf-8') as f:
        emailBody = f.read()

    
    #emailBody = """
    #<html>
    #<body>
    #<p>Olá, {receiverName}</p>
    #<p>Obrigado por participar da cabine de fotos NovaGenesis no Hacktown 2023! Você acaba de colaborar com o desenvolvimento da ciência e da tecnologia!</p>
    #<p>Você será conhecido por todos do Hacktown 2023. Procure nossa TV para verificar sua foto. Suas fotos também já estão no seu e-mail. Você pode baixá-las, compartilhá-las ou imprimi-las como quiser.</p>
    #<p>Esperamos que você tenha se divertido e aproveitado o evento. Fique ligado nas novidades da NovaGenesis, a internet do futuro!</p>
    #<p>Atenciosamente,</p>
    #<p>Equipe NovaGenesis</p>
    #</body>
    #</html>
    #""".format(receiverName=receiverName)

    mensagem.attach(MIMEText(emailBody, 'html'))

    # Take the photos and return the filenames
    filenames = capture_photo('')
    #filenames = capture_photo_vertically('')

    # Crop the photos to 9:16 aspect ratio
    for filename in filenames:
        crop_to_nine_sixteen(filename)

    # Copy the photos to the NovaGenesis folder
    copy_to_ng_folder(filenames)

    # Send the photos as attachments
    try:
        # Connect to the SMTP server
        server = smtplib.SMTP(smtpServer, smtpPort)
        server.starttls()

        # Perform Login
        server.login(senderEmail, senderPassw)

        # Attach the photos
        for filename in filenames:
            # Open the file
            filepath = os.path.join('', filename)
            with open(filepath, 'rb') as f:
                # Read the file content and encode to base64
                file_content = f.read()

                # Add the attachment
                attachment = MIMEBase('application', 'octet-stream')
                attachment.set_payload(file_content)
                encoders.encode_base64(attachment)
                attachment.add_header('Content-Disposition', 'attachment', filename=filename)
                mensagem.attach(attachment)

        # Send the email
        server.sendmail(senderEmail, receiverEmail, mensagem.as_string())

        # Close connection
        server.quit()

        print('E-mail enviado com sucesso!')
    except Exception as e:
        print('Ocorreu um erro ao enviar o e-mail:', str(e))

def copy_to_ng_folder(filenames):
    print("Copying photos to NovaGenesis folder...")
    for filename in filenames:
        # Copy the file from current folder to NovaGenesis folder
        shutil.copy(filename, ngFolder)
        print("Photo copied: " + filename)

    print("Photos copied.")

def load_config():
    print("Loading config...")
    with open('config.json', 'r') as f:
        config = json.load(f)
    print("Config loaded.")
    return config

# Define a route for the post API that only accepts requests from the same origin
@app.route('/takePicture', methods=['POST'])
@cross_origin(origin='localhost', headers=['Content-Type', 'Authorization'])
def post():
    # Try to acquire the lock
    if not lock.acquire(blocking=False):
        return flask.jsonify({'success': False})

    # Get the payload as JSON
    data = flask.request.get_json()
    # Get the name and email from the data
    name = data.get('name')
    email = data.get('email')

    # Send the email in a new thread
    send_email(email, name)

    # Release the lock
    lock.release()
    
    return flask.jsonify({'success': True})

# Run the app
if __name__ == '__main__':
    print("Starting the app...")
    config = load_config()
    senderEmail = config['email']['sender']
    senderPassw = config['email']['password']
    smtpServer = config['email']['smtpServer']
    smtpPort = config['email']['smtpPort']
    cameraIndex = config['camera']['index']
    cameraDelay = config['camera']['delay']
    filterImageName = config['camera']['filterImageName']
    ngFolder = config['folder']['ngPath']
    app.run()
