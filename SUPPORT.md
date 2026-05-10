# Auto Blinds Support

This document includes the usage of the code.

## Getting Started

### secret.h

A header file `secret.h` is necessary to run the code without error. This file includes sensitive information, so please don't forget to add it to `.gitignore` if you are using a git repository.

1. Create `secret.h` in either [\src](\src) or [/include](/include) directory.

2. Add the following code to your `secret.h`. Please note that this is a sample code. You need to adjust it according to your own situation.

```C++
#ifndef ARDUINO_SECRETS_H
#define ARDUINO_SECRETS_H

/* WiFi Configuration */
// WPA2-Enterprise WiFi
#define SECRET_SSID "Your_University_or_Office_SSID"
#define SECRET_USER "Your_WPA2_Username"
#define SECRET_PASS "Your_WPA2_Password"

/* Notion */
#define NOTION_TOKEN "Your_Notion_API_Integration_Token"
#define DATA_SOURCE_ID "Your_data_source_ID"
#define DATABASE_ID "Your_database_ID"

#define NOTION_PREVIEW_BLOCK_ID "Your_Notion_embed_webpage_block_ID"

/* Location */
#define OBSERVER_LATITUDE_DEG 37.334886             // Observer latitude in degrees (Positive: North, Negative: South)
#define OBSERVER_LONGITUDE_DEG -122.008988          // Observer longitude in degrees (Positive: East, Negative: West)
#define TIME_ZONE -8

/* Daily routine */
// Time to wake up
#define wakeup_hour 6
#define wakeup_min 0
#define wakeup_sec 0

// Time to go to sleep
#define sleep_hour 21
#define sleep_min 30
#define sleep_sec 0

#endif
```

### Notion Setup

![](/SUPPORT/Notion_setup_preview.png)

1. Create a Notion page wherever you want

2. Give it a name (e.g. `Auto Blinds`)

3. Add several buttons (e.g. `Close the Blinds`, `Open the Blinds`, `Block the Sun`, `Auto Mode`, etc.)

4. Copy and paste the url https://buttonbugs.github.io/Auto-Blinds/status/?auto_mode=true&angle=0&target=0&sun_u=0&sun_v=0&sun_w=1 into the page and select `Embed`

5. Type `/database` and select `Database - Inline`

6. Give it a name (e.g. `Command`)

7. Rename the current property `Name` to `Description`

8. Add the following properties to the database

| Property      | Type          | Notes     |
| -             | -             | -         |
| Target        | Number        | This is the angle you want the blinds to reach |
| Status        | Status        | This is the status of the commands |
| Created by    | Created by    | The person creating the command (**Optional**, but useful when multiple people have access to your Auto Blinds) |
| Created time  | Created time  | When the command is created (**Optional**, but useful when multiple people have access to your Auto Blinds) |

Note: Please **DO NOT** change the property name unless you change it in `notion_handler.cpp` as well.
