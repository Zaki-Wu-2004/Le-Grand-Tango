import serial
import time

# Set serial parameters
ser = serial.Serial('COM8', 9600, timeout=1)  # Replace with the actual serial port

time.sleep(2)  # Wait for Arduino to reset

def send_coordinates(coordinates):
    # Serialize the coordinate list into a string
    data = ';'.join(["{},{}".format(x, y) for x, y in coordinates])
    ser.write(data.encode())  # Send data
    time.sleep(0.5)
    response = ser.readline().decode('utf-8').strip()  # Read the response from Arduino
    return response

def get_user_coordinates():
    coordinates = []
    while True:
        user_input = input("Enter a set of 4 coordinate: ")

        try:
            a,b,c,d,e,f,g,h = map(int, user_input.split(' '))
            coordinates.append((a, b))
            coordinates.append((c, d))
            coordinates.append((e, f))
            coordinates.append((g, h))
            break
        except ValueError:
            print("Invalid input. Please enter coordinates as 'x,y'.")
    return coordinates

try:
    while True:
        # Example coordinate data
        #coordinates = [(0, 0), (-30, -30), (3, -32), (15, 0)]
        #0 0 -30 -30 3 -32 15 0 this is a set of coordinate that survives the test
        coordinates = get_user_coordinates()
        print(coordinates)
        print(f"Sending coordinates: {coordinates}")
        response = send_coordinates(coordinates)
        print(f"Arduino response: {response}")
        time.sleep(5)  # Send data every 5 seconds
except KeyboardInterrupt:
    print("Communication stopped by user.")
finally:
    ser.close()  # Close the serial port
