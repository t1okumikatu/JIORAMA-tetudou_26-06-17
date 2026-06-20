// 正転
void forward(uint32_t pwm) {
  if (pwm > VALUE_MAX) {
    pwm = VALUE_MAX;
  }
  ledcWrite(IN1, pwm); // ⭕ 大文字に修正
  ledcWrite(IN2, 0);   // ⭕ 大文字に修正
}

// 逆転
void reverse(uint32_t pwm) {
  if (pwm > VALUE_MAX) {
    pwm = VALUE_MAX;
  }
  ledcWrite(IN1, 0);   // ⭕ 大文字に修正
  ledcWrite(IN2, pwm); // ⭕ 大文字に修正
}

// ブレーキ
void brake() {
  ledcWrite(IN1, VALUE_MAX); // ⭕ 大文字に修正
  ledcWrite(IN2, VALUE_MAX); // ⭕ 大文字に修正
}

// 空転
void coast() {
  ledcWrite(IN1, 0); // ⭕ 大文字に修正
  ledcWrite(IN2, 0); // ⭕ 大文字に修正
}