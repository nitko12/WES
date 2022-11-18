#include <Arduino.h>
#include "calculate_angle.hpp"

const byte outPin = 6; // bobina
const byte debugPin = 4;

const byte koljenasto_vratilo_pin = 1;
const byte bregasta_osovina_pin = 3;
const byte MAP_pin = A0;

static uint32_t last_time, last_diff = 2000;

int step = 0;

byte pojavi_se = 1, state_pojavljivanja = 0;

uint32_t rpm = 1000, val = 10;

byte state = 0;

ISR(INT0_vect)
{

    uint32_t now = micros();
    uint32_t diff = now - last_time;

    if (diff > 10000)
    {
        diff = 6000;
    }

    if (2 * last_diff < diff)
    {
        rpm = 7500000 / diff;

        // OCR1A = (-diff * deg / 45 + 10 * diff / 3 - 1500) / 16;

        // PORTD &= ~(1 << outPin);

        // TCNT1 = 0;
        // TCCR1B |= (1 << CS12);

        step = 0;

        state_pojavljivanja ^= 1;
    }

    if (step == 15)
    {
        uint32_t iduci_deseti = 18 * diff;

        iduci_deseti -= 1500 + 100;

        uint32_t deg = val;

        uint32_t deg_time = deg * diff / 15;

        iduci_deseti -= deg_time;

        iduci_deseti /= 16;
        iduci_deseti -= 1;

        // Serial.println(iduci_deseti);
        // Serial.println(val);
        // Serial.println(deg);
        // Serial.println(rpm);
        // Serial.println(diff);

        if (pojavi_se & (1 << state_pojavljivanja))
        {
            TCCR1B = 0;

            TCNT1L = 0;
            TCNT1H = 0;

            OCR1AH = iduci_deseti >> 8;
            OCR1AL = iduci_deseti & 0xff;

            PORTD &= ~(1 << outPin);

            TCCR1B |= (1 << CS12);
        }
    }

    ++step;
    last_time = now;
    last_diff = diff;
}

uint32_t diff_bre, last_time_bre;

byte bio_prvi_bre = 0;

ISR(INT1_vect)
{
    uint32_t now = micros();
    diff_bre = now - last_time_bre;
    last_time_bre = now;

    ++bio_prvi_bre;
    if (bio_prvi_bre >= 2)
    {
        bio_prvi_bre = 2;
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (state == 0)
    {
        OCR1AH = 0;
        OCR1AL = 93;

        PORTD |= (1 << outPin);
        state = 1;

        TCNT1L = 0;
        TCNT1H = 0;
        TCCR1B |= (1 << CS12);

        // Serial.println(micros());
        // Serial.print(micros());
    }
    else
    {
        PORTD &= ~(1 << outPin);
        state = 0;

        TCCR1B &= ~(1 << CS12);
    }
}

void setup()
{
    pinMode(outPin, OUTPUT);
    pinMode(debugPin, OUTPUT);
    pinMode(koljenasto_vratilo_pin, INPUT);
    pinMode(bregasta_osovina_pin, INPUT_PULLUP);
    pinMode(MAP_pin, INPUT);

    cli();

    // first signal interrupt

    EICRA = 0;
    EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC11);
    EIMSK |= bit(INT0) | bit(INT1);

    // second interrupt

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1AH = 0;
    OCR1AL = 93;

    TCCR1B |= (1 << WGM12);

    TCCR1B = 0;

    TIMSK1 |= (1 << OCIE1A);

    sei();

    PORTD &= ~(1 << debugPin);

    // Serial.begin(115200);
}

void loop()
{
    int MAP = map(map(analogRead(A0), 0, 1023, 0, 5000), 500, 4500, 15, 100);

    val = get_value(rpm, MAP);

    if (bio_prvi_bre == 2 && millis() > 2000 && micros() - last_time_bre > diff_bre * 3)
    {
        pojavi_se = 3;
    }
    else
    {
        pojavi_se = 1;
    }

    // Serial.println(bio_prvi_bre);
    // Serial.println(micros());
    // Serial.println(last_time_bre);
    // Serial.println(diff_bre);
    // Serial.println();
    // Serial.println();
}