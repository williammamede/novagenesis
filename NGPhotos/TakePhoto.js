
// Function to play the audio
function playAudio() {
    // Disable the button
    disableAudioButton();
    // open audio from local file audioTest.mp3
    const audio = new Audio('audioTest.mp3');
    audio.play();
    // Wait audio to finish and then show the form
    audio.onended = function () {
        // Show the form
        document.getElementById('formBox').style.display = 'block';
        // Hide the initial info
        document.getElementById('initialInfo').style.display = 'none';
    };
}

function disableAudioButton() {
    const audioButton = document.getElementById('audioButton');
    audioButton.disabled = true;
    audioButton.style.backgroundColor = '#ccc';
    audioButton.style.cursor = 'not-allowed';
}

function enableAudioButton() {
    const audioButton = document.getElementById('audioButton');
    audioButton.disabled = false;
    audioButton.style.backgroundColor = '#4CAF50';
    audioButton.style.cursor = 'pointer';
}

// Function to start capturing the webcam image
function startCapture() {
    // Check if the "Permitir tirar a foto" checkbox is checked
    const allowCaptureCheckbox = document.getElementById('allowCapture');
    if (!allowCaptureCheckbox.checked) {
        alert('Por favor, marque a caixa "Permitir tirar a foto" antes de começar.');
        return;
    }

    // Post the api http://127.0.0.1:5000/takePicture passing the name and email
    const nameInput = document.getElementById('name');
    const emailInput = document.getElementById('email');
    const data = {
        name: nameInput.value,
        email: emailInput.value
    };
    startPhotoCaptureTimer();
}

function startPhotoCaptureTimer() {
    var photosToCapture = 3;
    var capturedPhotos = 0;
    var count = 0;
    var intervalBetweenPhotos = 3;
    var timer = document.getElementById('timer');
    var timerContainer = document.getElementById('timer-container');
    timerContainer.style.display = 'block';

    // Hide the form
    document.getElementById('formBox').style.display = 'none';

    // Show the webcam preview
    const videoCamera = document.getElementById('videoCamera');
    videoCamera.style.display = 'block';

    var intervalId;
    function updateTimer() {
        if (capturedPhotos == photosToCapture) {
            reset();
        }
        if (count == intervalBetweenPhotos) {
            capturedPhotos += 1;
            blinkScreen();
            capturePhoto();
        }
        count = count == (intervalBetweenPhotos || capturedPhotos == photosToCapture) ? count = 0 : count + 1;
        timer.innerText = count;
    }

    function blinkScreen() {
        var body = document.getElementsByTagName('body')[0];
        body.classList.add('blinking');
        setTimeout(function () {
            body.classList.remove('blinking');
        }, 700);
    }

    function reset() {
        clearInterval(intervalId);
        document.getElementById('name').value = '';
        document.getElementById('email').value = '';
        document.getElementById('allowCapture').checked = false;
        checkFormValidity()
        timerContainer.style.display = 'none';
        // Close the webcam preview
        const videoCamera = document.getElementById('videoCamera');
        videoCamera.style.display = 'none';

        // Show last message for 10 seconds
        document.getElementById('lastMessage').style.display = 'block';
        setTimeout(function () {
            document.getElementById('lastMessage').style.display = 'none';
            document.getElementById('initialInfo').style.display = 'block';
            enableAudioButton();
        }, 15000);

    }

    intervalId = setInterval(updateTimer, 1000);
}

function capturePhoto() {
    // Open the webcam preview using node-webcam
    /* var NodeWebcam = require("node-webcam");
        var opts = {
            width: 1280,
            height: 720,
            quality: 100,
            delay: 0,
            saveShots: true,
            output: "jpeg",
            device: false,
            callbackReturn: "location",
            verbose: false
        };
        var Webcam = NodeWebcam.create(opts);
        const photoName = new Date().getTime();
        Webcam.capture(photoName, function (err, data) {
            if (err) {
                console.log(err);
            } else {
                console.log(data);
            }
        });
        Webcam.list(function (list) {
            var anotherCam = NodeWebcam.create({ device: list[0] });
        }); */
    const canvas = document.createElement("canvas");
    const context = canvas.getContext("2d");
    const video = document.getElementById("videoCamera");
    const fs = require('fs');
    const path = require('path');

    canvas.width = video.videoWidth;
    canvas.height = video.videoHeight;
    context.drawImage(video, 0, 0, canvas.width, canvas.height);

    const dataURL = canvas.toDataURL("image/jpeg");

    const fileName = new Date().toISOString() + ".jpg";
    const filePath = path.join(__dirname, fileName);

    // Save the photo to the disk
    fs.writeFile(filePath, dataURL.replace(/^data:image\/jpeg;base64,/, ""), 'base64', { flag: "wx" }, function (err, data) {
        if (err) {
            console.log(err);
        } else {
            console.log("The file was created!");
        }
    });
}

function sendEmail(data, phtoNames) {
    // Send email with the photos

}
// Function to check form validity and enable/disable the "Começar" button
function checkFormValidity() {
    const nameInput = document.getElementById('name');
    const emailInput = document.getElementById('email');
    const allowCaptureCheckbox = document.getElementById('allowCapture');
    const capturarButton = document.getElementById('capturarButton');

    if (nameInput.checkValidity() && emailInput.checkValidity() && allowCaptureCheckbox.checked) {
        capturarButton.disabled = false;
    } else {
        capturarButton.disabled = true;
    }
}

// Add event listeners to form fields to check validity on input changes
document.getElementById('name').addEventListener('input', checkFormValidity);
document.getElementById('email').addEventListener('input', checkFormValidity);
document.getElementById('allowCapture').addEventListener('change', checkFormValidity);

// Disabble form and last message
document.getElementById('formBox').style.display = 'none';
document.getElementById('lastMessage').style.display = 'none';



// Open the webcam for preview and crop the image to 
const videoCamera = document.getElementById('videoCamera');
const constraints = {
    video: {
        width: { min: 270, ideal: 270, max: 270 },
        height: { min: 480, ideal: 480, max: 480 }
    }
};
navigator.mediaDevices.getUserMedia(constraints)
    .then(stream => {
        videoCamera.srcObject = stream;
        videoCamera.play();
    })
    .catch(error => {
        console.error(error);
    });
