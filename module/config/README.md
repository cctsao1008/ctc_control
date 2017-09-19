# libconfig
 * This is a slight lib to support quickly create/read/write parameters in a config file.

 * The config may have several sections defined within [ ].
One section may have several key-value pairs defined below section name.
Config, section, key may have comment above there definitions.
Four types of value are supported: string, integer, float, boolean.

 * An octothorp(#) or asterisk(*) started string is regarded as comment.
 The key-value pair should/will be separated by equal sign(=).
 String "True", "Yes", "1" are regarded as positive value for boolean while "False", "No", "0" for negative.
 See "example.ini" for reference.

 * See annotation of each functions for detail and "example.c" for usage.
