# MLB LCD Dashboard

This experimental code pulls a data feed of major league baseball games and sends it to the https://www.particle.io/ API to display stats on an LCD screen.

![Particle and LCD](https://github.com/topfunky/particle-lcd-mlb-dashboard/raw/master/assets/sparkfun-lcd-mlb.jpg)

# Development Notes

## Setup

Install [Particle CLI](https://docs.particle.io/tutorials/developer-tools/cli/#installing).

```
bash <( curl -sL https://particle.io/install-cli )
```

Optionally install [dfu-util](https://docs.particle.io/support/particle-tools-faq/installing-dfu-util/)

```
brew install dfu-util
```

## Hardware

- `5v` out of LCD panel plugs into the `3v3` pin
- `GND` plugs into any ground
- `RX` out of LCD panel plugs into `TX` on the Photon
- End pin of 3rd base LED goes to `A6`. End pin of 2nd base goes to `A5`. End pin of 1st base goes to `A4`.
- LEDs need resistor to `GND`.

## API

Send a full score to both lines of the LCD:

```
curl -H "Authorization: Bearer $SPARK_ACCESS_TOKEN" https://api.particle.io/v1/devices/$SPARK_DEVICE_ID/lcdRow1 -d arg="*NYY 10 Judg 1-2 LAD 12    8  **"
```

Turn on an LED for all bases (321).

```
curl -H "Authorization: Bearer $SPARK_ACCESS_TOKEN" https://api.particle.io/v1/devices/$SPARK_DEVICE_ID/bases -d arg="0000000000000321"
```

## MLB Data

```json
away_name_abbrev
home_name_abbrev
status (strikes, balls, etc.)

status:
     { b: '2',
       ind: 'I',
       inning: '4',
       inning_state: 'Top',
       is_no_hitter: 'N',
       is_perfect_game: 'N',
       note: '',
       o: '0',
       reason: '',
       s: '1',
       status: 'In Progress',
       top_inning: 'Y' },

linescore
```
