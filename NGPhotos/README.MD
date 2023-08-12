# NGPhotos
## 1. Introduction
This is a NovaGesis application which takes photos from a camera send them through the NG Network and send the photos by email.

## 2. Installation
### 2.1. Install the NG Network
Install node.js and npm. by running the following commands:
```
sudo apt-get install nodejs
sudo apt-get install npm
```
Install electron by running the following command:
```
sudo npm install -g electron
```
Install fs and path by running the following command:
```
sudo npm install fs
sudo npm install path
```
Install webcamjs by running the following command:
```
sudo npm install webcamjs
```
Install jimp by running the following command:
```
sudo npm install jimp
```
Install nodemailer by running the following command:
```
sudo npm install nodemailer
```

### 2.2. Set the environment variables
Set the environment variables by running the following commands:
```
export NG_EMAIL="YOUR EMAIL"
export NG_EMAIL_PASSWORD="YOUR EMAIL PASSWORD"
export NG_BASE="NOVAGESIS BASE FOLDER"
```

### 2.3. Run the application
Run the application by running the following command:
```
npm start
```

### 2.4. Package the application
Package the application by running the following command:
```
npm run package
```

## 3. Usage
### 3.1. Listen to the explaination audio
Listen to the explaination audio by clicking on the play button.

### 3.2. Fill the form and take a photo
Fill the form with name, email and mark the checkbox. Then click on the take photo button to take a photo. After that tree photos will be taken and will be sent by email and copied to the NG Network.

## 4. License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.