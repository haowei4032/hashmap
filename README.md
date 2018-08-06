# HashMap

PHP7 extension


Installation
------------

```
phpize
./configure --with-php-config=/etc/php/bin/php-config --enable-hashmap
make
make install
```



PHP API
------------

```php
<?php

class HashMap
{
    public function __construct() {}
    
    /**
     * @param string $key
     * @param mixed $value 
     * @return bool 
     */
    public function put($key, $value) {}
    
    /**
     * @param string $key
     * @return mixed 
     */
    public function get($key) {}
    
    /**
     * @param string $key
     * @return bool 
     */
    public function exists($key) {}
    
    /**
     * @param string $key
     * @return bool 
     */
    public function remove($key) {}
}

```