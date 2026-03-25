// my_helpers.h
#include "esphome.h"
#include <vector>

static std::vector<uint8_t> prepare_can_packet_set_bit(int cmd, int bit, uint16_t conf, bool val) {
    uint16_t new_conf = conf;
    
    if (val) {
        new_conf |= (1U << bit);
    } else {
        new_conf &= ~(1U << bit);
    }
    
    uint8_t b_low = (uint8_t)(new_conf & 0xFF);
    uint8_t b_high = (uint8_t)((new_conf >> 8) & 0xFF);
    
    ESP_LOGI("SETBIT", "Config 0x%02X: %04X -> %04X (bit %d set to %d)", cmd, conf, new_conf, bit, val);
	
    return {(uint8_t)cmd, 0x00, b_low, b_high};
}

static std::vector<uint8_t> prepare_can_packet_set_2bits(int cmd, int start_bit, uint16_t conf, int val) {
    uint16_t new_conf = conf;
    
    // 1. Создаем маску для двух бит (3 в десятичной это 11 в двоичной)
    // Сдвигаем её к нужному начальному биту. Например, если start_bit = 2, 
    // маска будет ...0001100
    uint16_t mask = 0b11 << start_bit;
    
    // 2. Очищаем эти два бита в текущем конфиге (ставим их в 0)
    new_conf &= ~mask;
    
    // 3. Записываем новое значение (val берем только младшие 2 бита и сдвигаем)
    new_conf |= ((uint16_t)(val & 0b11) << start_bit);
    
    uint8_t b_low = (uint8_t)(new_conf & 0xFF);
    uint8_t b_high = (uint8_t)((new_conf >> 8) & 0xFF);
    
    ESP_LOGI("SET2BIT", "Cmd 0x%02X: %04X -> %04X (bits %d-%d set to %d); New L %02X - H %04X", 
             cmd, conf, new_conf, start_bit, start_bit + 1, val, b_low, b_high);
	
    return {(uint8_t)cmd, 0x00, b_low, b_high};
}

float update_sensor_f001(esphome::sensor::Sensor *obj, std::vector<uint8_t> x) {
  if (x.size() >= 4) {
	float value = 0.01f * (x[2] + 256.0f * x[3]);
    obj->publish_state(value);
	return value;
  }
  
  return NAN;
}

float update_sensor_f001(esphome::number::Number *obj, const std::vector<uint8_t> &x) {
  if (x.size() >= 4) {
	float value = 0.01f * (x[2] + 256.0f * x[3]);
	obj->publish_state(value);
	return value;
  }
  
  return NAN;
}


float update_sensor_f01(esphome::sensor::Sensor *obj, std::vector<uint8_t> x) {
  if (x.size() >= 4) {
	float value = 0.1f * (x[2] + 256.0f * x[3]);
    obj->publish_state(value);
	return value;
  }
  
  return NAN;
}

int update_sensor_int16(esphome::number::Number *obj, const std::vector<uint8_t> &x) {
  if (x.size() >= 4) {
	int value = x[2] + 256 * x[3];
	obj->publish_state(value);
	return value;
  }
  
  return 0;
}


int update_sensor_byte(esphome::template_::TemplateSwitch *obj, std::vector<uint8_t> x) {
  if (x.size() >= 3) {
    obj->publish_state(x[2]);
	return x[2];
  }
  
  return 0;
}