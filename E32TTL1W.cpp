#include "E32TTL1W.hpp"

E32TTL1W::E32TTL1W(Stream &stream, uint8_t m0, uint8_t m1, uint8_t aux)
    : _stream(stream), _m0(m0), _m1(m1), _aux(aux)
{
  pinMode(_m0, OUTPUT);
  pinMode(_m1, OUTPUT);
}

bool E32TTL1W::readConfig(Config &config)
{
  return (_readInfo(Head::CFG_READ, (uint8_t *)&config));
}

bool E32TTL1W::writeConfig(Config &config)
{
  bool rslt = false;

  Mode prevMode = _mode;
  setMode(Mode::SLEEP);

  _stream.write((uint8_t *)&config, 6);
  delay(100);

  Config currentConfig;
  if (!readConfig(currentConfig))
    rslt = false;
  else
    rslt = !memcmp(&config, &currentConfig, 6);

  setMode(prevMode);
  return (rslt);
}

bool E32TTL1W::readVersion(Version &version)
{
  return (_readInfo(Head::VERSION_READ, (uint8_t *)&version));
}

E32TTL1W::Mode E32TTL1W::getMode()
{
  return (_mode);
}

void E32TTL1W::setMode(Mode mode)
{
  if (_mode == mode)
    return;

  _stream.flush();

  switch (mode)
  {
  case Mode::NORMAL:
    digitalWrite(_m0, LOW);
    digitalWrite(_m1, LOW);
    break;

  case Mode::WAKE_UP:
    digitalWrite(_m0, HIGH);
    digitalWrite(_m1, LOW);
    break;

  case Mode::POWER_SAVING:
    digitalWrite(_m0, LOW);
    digitalWrite(_m1, HIGH);
    break;

  case Mode::SLEEP:
    digitalWrite(_m0, HIGH);
    digitalWrite(_m1, HIGH);
    break;

  default:
    return;
  }
  delay(100);

  _mode = mode;
}

void E32TTL1W::reset()
{
  Mode prevMode = _mode;
  setMode(Mode::SLEEP);
  Head cmd[] = {Head::MODULE_RESET, Head::MODULE_RESET, Head::MODULE_RESET};
  _stream.write((uint8_t *)cmd, 3);
  delay(100);
  setMode(prevMode);
}

bool E32TTL1W::_readInfo(Head head, uint8_t *dest)
{
  uint8_t size = 0;
  switch (head)
  {
  case Head::CFG_READ:
    size = 6;
    break;

  case Head::VERSION_READ:
    size = 4;
    break;

  default:
    return (false);
  }

  Mode prevMode = _mode;
  setMode(Mode::SLEEP);

  while (_stream.available())
    _stream.read();

  Head cmd[] = {head, head, head};
  _stream.write((uint8_t *)cmd, 3);
  delay(100);

  uint8_t i = 0;
  uint8_t buffer[size];

  while (_stream.available() && i < size)
  {
    uint8_t byte = _stream.read();
    buffer[i++] = byte;
  }

  setMode(prevMode);

  if (i != size)
    return (false);

  memcpy(dest, buffer, size);

  return (true);
}