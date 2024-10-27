# Steganography
This project implements digital Least Significant Bit (LSB) steganography to hide secret data within a .bmp image file using C programming. By subtly altering the least significant bits of the image pixels, this technique embeds data without noticeable changes to the image's appearance, making it ideal for secure and unobtrusive data storage.

some key features Steganography project are as follows,

1. Conceal Any Data Format: Embed a variety of data types, such as text files, images, or other binary data, within the .bmp image.
2. Command-Line Interface: Provides straightforward command-line options to specify the image file, secret data, and output file.
3. Efficient File Management: Uses C file I/O for handling .bmp images and the encoded data seamlessly.
4. Structured Code: Employs custom data structures (structures and enums) for organized, expandable code, enhancing both functionality and readability.

How It Works:
The program reads the .bmp image, modifies its least significant bits to encode data, and outputs an encoded image with the hidden information embedded within. This approach makes the secret data difficult to detect without decoding.

How LSB Steganography Works:
In this project, digital LSB steganography modifies the least significant bit of each pixel in the .bmp image to embed data in a visually undetectable way.

Steganography involves Encoding of secret data into image and Decoding (extracting this secret data from the image).
1. Encoding Process: The program reads the .bmp image, and after the standard 54-byte header, it adds a unique magic string. This magic string acts as an identifier, ensuring that only intended recipients with the decoding tool recognize and extract the hidden data, preventing accidental misuse or misinterpretation.
2. Decoding Process: When decoding, the program scans for this magic string. Once detected, it proceeds to read and reconstruct the hidden data. Without this identifier, other programs will not recognize or attempt to access the hidden data.

With the help of this project, we can securely embed sensitive data within an image, while the magic string offers an extra layer of security by ensuring that only the intended parties can access the embedded data, protecting against unintended access or misuse.
