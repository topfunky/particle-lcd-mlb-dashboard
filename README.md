# MLB LCD Dashboard

This experimental code pulls a data feed of major league baseball games and sends it to the https://www.particle.io/ API to display stats on an LCD screen.



# Development Notes

## API

    curl -H "Authorization: Bearer $SPARK_TOKEN" https://api.particle.io/v1/devices/$SPARK_DEVICE_ID/lcdRow2 -d arg=" LAA 1     9 ** "

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


