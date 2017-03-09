#include <mbed.h>
#include <CircularBuffer.h>

template <int RXSize>
class BufferedRXSerial : public RawSerial {
public:
    BufferedRXSerial(PinName tx, PinName rx, int baud=MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
        : RawSerial(tx, rx, baud) {
        attach(Callback<void()>(this, &BufferedRXSerial<RXSize>::handleRxIrq));
    };

    int readable() {
        // TODO: use a circular buffer that exposes the number of available elements
        return rxBuffer.empty() ? 0 : 1;
    };

    int getc() {
        char datum = 0;
        if (rxBuffer.pop(datum)) {
            return datum;
        }
        return 0;
    };

    void handleRxIrq() {
        if (RawSerial::readable()) {
            char datum = static_cast<char>(RawSerial::getc());
            if (!rxBuffer.full()) {
                rxBuffer.push(datum);
            }
        }
    };
private:
    CircularBuffer<char, RXSize, int> rxBuffer;
};
