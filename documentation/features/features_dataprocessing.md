:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Data processing

## Lightweight XML parser
As of EDGELIB 3.30, a lightweight, native, event-based XML parser is included in the SDK to process documents formatted with the open standard Extensible Markup Language (XML). The XML parser notifies the application by firing XML events to handle your data easily, without the difficulties of parsing the text data yourself.

A good example of using XML is to store configuration files in a readable (and human editable) format, or to process XML-RPC webservice calls.


## Built-in XML-RPC parser
EDGELIB 3.30 introduced XML-RPC support as an additional class utilizing the built-in XML class. Now developers can easily interface with XML-RPC web services and create advanced online applications running from mobile devices.

Use the XML-RPC class to parse incoming responses and browse the data tree quickly. The class also adds error handling improving stability of your application.


## Data management class
As of EDGELIB 3.30, a data class is added to safely handle (large) data arrays and makes it easier without having to use complicated memory allocation and deallocation routines. Using data classes makes your application more stable.

Data classes are able to expand automatically when more data is needed. It will prevent you from having to reserve more memory than actually needed, making your application more memory efficient.
