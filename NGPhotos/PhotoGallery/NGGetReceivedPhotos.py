from flask import Flask, jsonify
import os
import random

from flask_cors import cross_origin

app = Flask(__name__)

fotos = []

@app.route('/getPhotosNames')
@cross_origin()
def get_photos_names():
    fotos = []

    path = "."
    for filename in os.listdir(path):
        if filename.endswith(".jpg"):
            fotos.append(filename)

    random.shuffle(fotos)


    return jsonify(fotos)

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5001))
    app.run(debug=True, port=port)
