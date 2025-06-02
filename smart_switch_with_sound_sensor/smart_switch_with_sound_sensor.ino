// Define the analog pin where the sound sensor's analog output is connected
const int soundSensorPin = A0;

// Define the digital pin where the device (e.g., an LED) is connected
const int devicePin = 2;

/*
 * Define a threshold for sound detection.
 * This value will vary depending on your sensor and environment.
 * You might need to adjust this by observing sensor readings in the Serial Monitor.
 */
const int soundThreshold = 500; // Adjustable according to your sensor and noise level

// Variable to keep track of the device's current state (true = ON, false = OFF)
bool deviceState = false;

// Variable to store the last time the device state was toggled
unsigned long lastToggleTime = 0;

// Debounce delay to prevent rapid toggling from continuous sound
// This means the device won't toggle again for at least this duration after a sound is detected.
// A longer delay (e.g., 500ms to 1000ms) is often better for sound detection.
const unsigned long debounceDelay = 500; // Changed to 500ms (0.5 seconds) - adjust as needed

// NEW: Variable to track the previous state of sound detection (was it above threshold or not?)
bool soundDetectedPreviously = false;

void setup() {
  // Initialize serial communication for debugging and monitoring sensor values
  Serial.begin(9600);
  Serial.println("Arduino Sound Sensor Device Control");

  // Set the device pin as an output
  pinMode(devicePin, OUTPUT);

  // Initially turn the device OFF
  digitalWrite(devicePin, LOW);
  Serial.println("Device is initially OFF.");
}

void loop() {
  // Read the analog value from the sound sensor
  int soundValue = analogRead(soundSensorPin);

  // Print the raw sound sensor value to the Serial Monitor (for calibration/debugging)
  // You can comment this out once you've set your soundThreshold
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  // Determine if the current sound value is above the threshold
  bool currentSoundIsAboveThreshold = (soundValue > soundThreshold);

  // Check for a "rising edge" (sound just crossed the threshold from below)
  // AND ensure enough time has passed since the last toggle
  if (currentSoundIsAboveThreshold && !soundDetectedPreviously && (millis() - lastToggleTime > debounceDelay)) {
    // A distinct sound event (rising edge) was detected, and it's time to toggle the device

    // Toggle the device state
    deviceState = !deviceState;

    // Update the physical device (LED) based on the new state
    if (deviceState) {
      digitalWrite(devicePin, HIGH); // Turn device ON
      Serial.println("Sound detected! Device turned ON.");
    } else {
      digitalWrite(devicePin, LOW); // Turn device OFF
      Serial.println("Sound detected! Device turned OFF.");
    }

    // Record the time of this toggle
    lastToggleTime = millis();
  }

  // Update the 'soundDetectedPreviously' for the next loop iteration
  soundDetectedPreviously = currentSoundIsAboveThreshold;

  // Small delay to prevent the loop from running too fast,
  // which can make serial output unreadable and consume unnecessary power.
  delay(50);
}
