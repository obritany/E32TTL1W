# E32TTL1W
Class for setting the parameters of the [E32-TTL-1W](https://www.cdebyte.com/products/E32-433T30D) radio module.

Includes a set of allowable parameters and functions for configuring the module.

## How to use
    E32TTL1W radio(Serial, 1, 2, 3);
    E32TTL1W::Config currentConfig;
    radio.readConfig(currentConfig);
    
    E32TTL1W::Config newConfig = currentConfig;
    newConfig.option.power = E32TTL1W::Power::_30;
    radio.writeConfig(newConfig);
