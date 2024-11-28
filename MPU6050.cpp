#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

class MPU6050 {
private:
    static const int MPU_ADDR = 0x68; // MPU-6050 I2C address
    int i2c_fd;

    // Convert int16 to formatted string
    std::string convert_int16_to_str(int16_t i) {
        char tmp_str[7];
        snprintf(tmp_str, sizeof(tmp_str), "%6d", i);
        return std::string(tmp_str);
    }

public:
    // Constructor
    MPU6050(const char* i2c_device = "/dev/i2c-1") {
        // Open I2C device
        i2c_fd = open(i2c_device, O_RDWR);
        if (i2c_fd < 0) {
            throw std::runtime_error("Failed to open I2C device");
        }

        // Set I2C slave address
        if (ioctl(i2c_fd, I2C_SLAVE, MPU_ADDR) < 0) {
            close(i2c_fd);
            throw std::runtime_error("Failed to set I2C slave address");
        }
    }

    // Destructor
    ~MPU6050() {
        close(i2c_fd);
    }

    // Initialize MPU-6050
    void initialize() {
        // Wake up MPU-6050
        uint8_t wake_cmd[2] = {0x6B, 0x00};
        if (write(i2c_fd, wake_cmd, 2) != 2) {
            throw std::runtime_error("Failed to wake up MPU-6050");
        }

        // Verify connection
        uint8_t who_am_i_reg = 0x75;
        uint8_t who_am_i_value;
        
        if (write(i2c_fd, &who_am_i_reg, 1) != 1) {
            throw std::runtime_error("Failed to select WHO_AM_I register");
        }

        if (read(i2c_fd, &who_am_i_value, 1) != 1) {
            throw std::runtime_error("Failed to read WHO_AM_I register");
        }

        if (who_am_i_value != 0x68) {
            throw std::runtime_error("MPU-6050 not found!");
        }

        std::cout << "MPU-6050 is connected." << std::endl;
    }

    // Read sensor data
    void read_sensor_data() {
        uint8_t data_reg = 0x3B; // Start of sensor data
        uint8_t buffer[14];

        // Write the register we want to read from
        if (write(i2c_fd, &data_reg, 1) != 1) {
            throw std::runtime_error("Failed to select data register");
        }

        // Read 14 bytes of data
        if (read(i2c_fd, buffer, 14) != 14) {
            throw std::runtime_error("Failed to read sensor data");
        }

        // Combine high and low bytes for each sensor reading
        int16_t accelerometer_x = (buffer[0] << 8) | buffer[1];
        int16_t accelerometer_y = (buffer[2] << 8) | buffer[3];
        int16_t accelerometer_z = (buffer[4] << 8) | buffer[5];

        int16_t temperature = (buffer[6] << 8) | buffer[7];

        int16_t gyro_x = (buffer[8] << 8) | buffer[9];
        int16_t gyro_y = (buffer[10] << 8) | buffer[11];
        int16_t gyro_z = (buffer[12] << 8) | buffer[13];

        // Print sensor data
        std::cout << std::fixed << std::setprecision(2)
                  << "Accelerometer: "
                  << "X=" << convert_int16_to_str(accelerometer_x)
                  << " Y=" << convert_int16_to_str(accelerometer_y)
                  << " Z=" << convert_int16_to_str(accelerometer_z)
                  << " | Temp: " << (temperature / 340.0 + 36.53)
                  << " | Gyroscope: "
                  << "X=" << convert_int16_to_str(gyro_x)
                  << " Y=" << convert_int16_to_str(gyro_y)
                  << " Z=" << convert_int16_to_str(gyro_z)
                  << std::endl;
    }
};

int main() {
    try {
        MPU6050 mpu;
        mpu.initialize();

        // Continuous reading loop
        while (true) {
            mpu.read_sensor_data();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}