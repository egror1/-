"""Run against an existing compiled executable: python3 tests/test_clock.py build/clock."""

from pathlib import Path
import subprocess
import sys


PROMPT = "Введите часы и минуты: "


def expected_time(hour, minute):
    """Independent oracle using explicit forms for the possible hour values."""
    if (hour, minute) == (0, 0):
        return "полночь"
    if (hour, minute) == (12, 0):
        return "полдень"
    hour_forms = (
        "12 часов", "1 час", "2 часа", "3 часа", "4 часа", "5 часов",
        "6 часов", "7 часов", "8 часов", "9 часов", "10 часов", "11 часов",
    )
    period = next(name for end, name in (
        (5, "ночи"), (12, "утра"), (18, "дня"), (24, "вечера")
    ) if hour < end)
    if minute == 0:
        return f"{hour_forms[hour % 12]} {period} ровно"
    one_minutes = {1, 21, 31, 41, 51}
    few_minutes = {2, 3, 4, 22, 23, 24, 32, 33, 34, 42, 43, 44, 52, 53, 54}
    minute_form = "минута" if minute in one_minutes else (
        "минуты" if minute in few_minutes else "минут"
    )
    return f"{hour_forms[hour % 12]} {minute} {minute_form} {period}"


def run(executable, input_text):
    result = subprocess.run(
        [str(executable)], input=input_text, capture_output=True,
        text=True, encoding="utf-8", timeout=5,
    )
    if not result.stdout.startswith(PROMPT):
        raise AssertionError(f"Missing prompt: {result.stdout!r}")
    return result.returncode, result.stdout[len(PROMPT):].strip(), result.stderr


def main():
    if len(sys.argv) != 2:
        raise SystemExit("Usage: python3 tests/test_clock.py path/to/executable")
    executable = Path(sys.argv[1]).resolve()
    passed = 0
    for hour in range(24):
        for minute in range(60):
            input_text = f"{hour} {minute}\n"
            code, output, stderr = run(executable, input_text)
            expected = expected_time(hour, minute)
            assert (code, output, stderr) == (0, expected, ""), (
                input_text, code, output, expected, stderr
            )
            passed += 1
    print(f"PASS: {passed} valid times (every minute of a full day)")
    invalid_range = (
        "-1 0\n", "24 3\n", "25 0\n", "0 -1\n", "0 60\n", "23 99\n",
    )
    invalid_syntax = (
        "", "abc\n", "3\n", "3 x\n", "2.5 10\n", "2 3.5\n",
        "2 3 text\n", "2 3 4\n", "999999999999999999999 0\n",
    )
    for text in invalid_range:
        code, output, stderr = run(executable, text)
        assert (code, output, stderr) == (1, "Введены недопустимые данные.", ""), (
            text, code, output, stderr
        )
    for text in invalid_syntax:
        code, output, stderr = run(executable, text)
        assert (code, output, stderr) == (1, "Ошибка: введите два целых числа.", ""), (
            text, code, output, stderr
        )
    for text in ("3 1", "3 1  \t\r\n", "03 01\n"):
        code, output, stderr = run(executable, text)
        assert (code, output, stderr) == (0, "3 часа 1 минута ночи", "")
    print(f"PASS: {len(invalid_range)} range errors and {len(invalid_syntax)} syntax errors")
    print("PASS: 3 accepted whitespace and leading-zero input forms")
    print(f"TOTAL: {passed + len(invalid_range) + len(invalid_syntax) + 3} passed")


if __name__ == "__main__":
    main()
