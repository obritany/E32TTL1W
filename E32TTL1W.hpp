/**
 * @file
 * @brief File defines the class E32TTL1W intended for use with E32-TTL-1W radio module.
 * @author obritany
 * @see https://www.cdebyte.com/products/E32-433T30D
 */

#ifndef E32TTL1W_HPP
#define E32TTL1W_HPP

#include <Arduino.h>

/**
 * @brief Class for setting the parameters of the E32-TTL-1W radio module.
 * @details Includes a set of allowable parameters and functions for configuring the module.
 * 
 * @code
 *  E32TTL1W radio(Serial, 1, 2, 3);
 *  E32TTL1W::Config currentConfig;
 *  radio.readConfig(currentConfig);
 *  E32TTL1W::Config newConfig = currentConfig;
 *  newConfig.option.power = E32TTL1W::Power::_30;
 *  radio.writeConfig(newConfig);
 * @endcode
 */
class E32TTL1W
{
public:
  /** @brief Command type */
  enum class Head : uint8_t
  {
    CFG_SAVE_HARD = 0xC0,
    CFG_READ,
    CFG_SAVE_SOFT,
    VERSION_READ,
    MODULE_RESET
  };

  /** @brief Parameter state for use in the Option structure */
  enum class State : uint8_t
  {
    DISABLE,
    ENABLE
  };

  enum class Parity : uint8_t
  {
    _8N1,
    _8O1,
    _8E1
  };

  enum class UartRate : uint8_t
  {
    _1200,
    _2400,
    _4800,
    _9600,
    _19200,
    _38400,
    _57600,
    _115200
  };

  enum class AirRate : uint8_t
  {
    _0k3,
    _1k2,
    _2k4,
    _4k8,
    _9k6,
    _19k2
  };

  enum class IoMode : uint8_t
  {
    OPEN_DRAIN,
    PUSH_PULL
  };

  enum class WorTiming : uint8_t
  {
    _250,
    _500,
    _750,
    _1000,
    _1250,
    _1500,
    _1750,
    _2000
  };

  enum class Power : uint8_t
  {
    _30,
    _27,
    _24,
    _21
  };

  enum class Mode : uint8_t
  {
    NORMAL,
    WAKE_UP,
    POWER_SAVING,
    SLEEP
  };

public:
  /** @brief Basic structure for storing parameters */
  struct Config
  {
  private:
    /** @brief Structure for SPED byte configuration */
    struct Sped
    {
      AirRate airRate : 3;   // bit 0-2
      UartRate uartRate : 3; // bit 3-5
      Parity parity : 2;     // bit 6-7
    };

    /** @brief Structure for OPTION byte configuration */
    struct Option
    {
      Power power : 2;         // bit 0-1
      State fec : 1;           // bit 2
      WorTiming worTiming : 3; // bit 3-5
      IoMode ioMode : 1;       // bit 6
      State fixedMode : 1;     // bit 7
    };

  public:
    Head head;
    uint8_t addH;
    uint8_t addL;
    Sped sped;
    uint8_t chan;
    Option option;
  };

  /** @brief Structure for storing version info */
  struct Version
  {
    Head head;
    uint8_t model;
    uint8_t version;
    uint8_t features;
  };

  /**
   * @brief Construct a new E32TTL1W object with empty Config structure.
   * @param stream Serial port to comunicate with radio module.
   * @param m0 Number of module M0 pin.
   * @param m1 Number of module M1 pin.
   * @param aux Number of module AUX pin.
   */
  E32TTL1W(Stream &stream, uint8_t m0, uint8_t m1, uint8_t aux);

  /**
   * @brief Function for reading module parameters into a Config structure.
   * @param config Reference to the Config structure for storing parameters.
   * @return Completion status.
   */
  bool readConfig(Config &config);

  /**
   * @brief Function for writing parameters into a module.
   * @param config Reference to the Config structure to write.
   * @return Completion status.
   */
  bool writeConfig(Config &config);

  /**
   * @brief Function for reading module version info into a Version structure.
   * @param config Reference to the Version structure for storing info.
   * @return Completion status.
   */
  bool readVersion(Version &version);

  /**
   * @brief Get current Mode of the radio module seted by m0, m1 pins.
   * @return Current Mode of the radio module.
   */
  Mode getMode();

  /**
   * @brief Set Mode of the radio module by m0, m1 pins.
   * @param mode Target Mode.
   */
  void setMode(Mode mode);

  /** @brief Reset radio module */
  void reset();

private:
  Stream &_stream;
  uint8_t _m0;
  uint8_t _m1;
  uint8_t _aux;
  Mode _mode;
  
  bool _readInfo(Head head, uint8_t *dest);
};

#endif // E32TTL1W_HPP