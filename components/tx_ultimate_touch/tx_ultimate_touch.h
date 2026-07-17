#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/uart/uart_component.h"

namespace esphome
{
    namespace tx_ultimate_touch
    {
        const static uint8_t TOUCH_STATE_RELEASE = 1;
        const static uint8_t TOUCH_STATE_PRESS = 2;
        const static uint8_t TOUCH_STATE_SWIPE = 3;

        const static uint8_t TOUCH_STATE_ALL_FIELDS = 11;

        const static uint8_t TOUCH_STATE_SWIPE_RIGHT = 12;
        const static uint8_t TOUCH_STATE_SWIPE_LEFT = 13;

        struct TouchPoint
        {
            int8_t x = -1;
            int8_t state = -1;
        };

        class TxUltimateTouch : public uart::UARTDevice, public Component
        {
        public:
            Trigger<TouchPoint> *get_touch_trigger() { return &this->touch_trigger_; }
            Trigger<TouchPoint> *get_release_trigger() { return &this->release_trigger_; }
            Trigger<TouchPoint> *get_swipe_left_trigger() { return &this->swipe_trigger_left_; }
            Trigger<TouchPoint> *get_swipe_right_trigger() { return &this->swipe_trigger_right_; }
            Trigger<TouchPoint> *get_full_touch_release_trigger() { return &this->full_touch_release_trigger_; }
            Trigger<TouchPoint> *get_long_touch_release_trigger() { return &this->long_touch_release_trigger_; }

            void set_uart_component(esphome::uart::UARTComponent *uart_component)
            {
                this->set_uart_parent(uart_component);
            }
            void set_require_press_before_release(bool require_press_before_release)
            {
                this->require_press_before_release_ = require_press_before_release;
            }
            void set_release_debounce(uint32_t release_debounce_ms)
            {
                this->release_debounce_ms_ = release_debounce_ms;
            }

            void setup() override;
            void loop() override;
            void dump_config() override;

        protected:
            void send_touch_(TouchPoint tp);
            void handle_touch(int bytes[]);

            TouchPoint get_touch_point(int bytes[]);
            bool is_valid_data(int bytes[]);
            int get_x_touch_position(int bytes[]);
            int get_touch_state(int bytes[]);

            bool is_orphaned_release_();

            Trigger<TouchPoint> touch_trigger_;
            Trigger<TouchPoint> release_trigger_;
            Trigger<TouchPoint> swipe_trigger_left_;
            Trigger<TouchPoint> swipe_trigger_right_;
            Trigger<TouchPoint> full_touch_release_trigger_;
            Trigger<TouchPoint> long_touch_release_trigger_;
            bool require_press_before_release_{false};
            bool has_pending_press_{false};
            uint32_t release_debounce_ms_{0};
            uint32_t last_release_ms_{0};
            bool has_last_release_{false};

        }; // class TxUltimateTouch

    } // namespace tx_ultimate_touch
} // namespace esphome
