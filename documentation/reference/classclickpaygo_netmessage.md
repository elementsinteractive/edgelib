:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions (deprecated)

When using ClickPayGo, it will inform the EDGELIB framework when requests are complete or an error has occured.

## ENET_CPGBALANCE
The balance of the user account is known. The data buffer contains the balance inside an [ETYPE_MONEY](ref_definitions.md) variable.

## ENET_CPGGROSSPRICE
The gross price of an item is known. The data buffer contains the price inside an ETYPE_MONEY variable.

## ENET_CPGVATPERC
The VAT percentage for the user account is known. The data buffer contains the percentage inside an ETYPE_MONEY variable.

## ENET_CPGTRANSACTION
The transaction is complete. The data buffer contains the 32-byte receipt code.

## ENET_CPGVALIDATED
The transaction is validated.

## ENET_CPGERROR
An error occured while using an API call.

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added to SDK

