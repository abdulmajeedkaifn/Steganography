# Steganography
Project Overview: This project is a digital Least Significant Bit (LSB) steganography tool built in C, designed to securely embed secret data within .bmp images. By modifying the LSBs of pixel data, it achieves data concealment with minimal effect on the image’s appearance, making the data difficult to detect. The program includes a built-in magic string after the 54-byte header of the .bmp file to identify and protect the hidden data from unintended access.

Project Objective: The objective of this project is to provide a way to securely transmit sensitive information using images as the carrier medium. The encoded image looks identical to the original, making it an ideal method for covert communication. Only someone with the appropriate decoding mechanism can extract the hidden data.

Key features Steganography project:

1. Conceal Any Data Format: Embed a variety of data types, such as text files, images, or other binary data, within the .bmp image.
2. Command-Line Interface: Provides straightforward command-line options to specify the image file, secret data, and output file.
3. Efficient File Management: Uses C file I/O for handling .bmp images and the encoded data seamlessly.
4. Structured Code: Employs custom data structures (structures and enums) for organized, expandable code, enhancing both functionality and readability.

How It Works:
The program reads the .bmp image, modifies its least significant bits to encode data, and outputs an encoded image with the hidden information embedded within. This approach makes the secret data difficult to detect without decoding.

How LSB Steganography Works:
In this project, digital LSB steganography modifies the least significant bit of each pixel in the .bmp image to embed data in a visually undetectable way.

1. Encoding Process: The program reads the .bmp image, and after the standard 54-byte header, it adds a unique magic string. This magic string acts as an identifier, ensuring that only intended recipients with the decoding tool recognize and extract the hidden data, preventing accidental misuse or misinterpretation.
2. Decoding Process: When decoding, the program scans for this magic string. Once detected, it proceeds to read and reconstruct the hidden data. Without this identifier, other programs will not recognize or attempt to access the hidden data.

With the help of this project, we can securely embed sensitive data within an image, while the magic string offers an extra layer of security by ensuring that only the intended parties can access the embedded data, protecting against unintended access or misuse.
