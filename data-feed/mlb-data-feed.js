const sprintf = require("sprintf-js").sprintf;
const Mlbgames = require("mlbgames");
const request = require("request");
const moment = require("moment-timezone");

const GAME_IN_PROGRESS = "In Progress";

const FAVORITE_TEAMS = [
  "CHC",
  "NYY",
  "LAA",
  "LAD",
  "WSH",
  "SEA",
  "OAK",
  "BOS",
  "MIL",
  "CWS",
  "NYM"
];

var today = moment().tz("America/Los_Angeles");

main(today);

function main(date) {
  var options = {
    path: date.format("[year_]YYYY/[month_]MM/[day_]DD/")
  };

  var mlbgames = new Mlbgames(options);
  mlbgames.get((err, games) => {
    var gamesWithFavoriteTeams = games.filter(
      game =>
        FAVORITE_TEAMS.includes(game.home_name_abbrev) ||
        FAVORITE_TEAMS.includes(game.away_name_abbrev)
    );

    if (gamesWithFavoriteTeams.length == 0) {
      return main(
        main(
          moment()
            .subtract(1, "days")
            .calendar()
        )
      );
    }

    var gamesInProgress = gamesWithFavoriteTeams.filter(
      game => game.status.status == GAME_IN_PROGRESS
    );

    if (gamesInProgress.length > 0) {
      // Sort by favorite teams
      var sortedGames = gamesInProgress.sort(function(obj1, obj2) {
        obj1HomeIndex = FAVORITE_TEAMS.indexOf(obj1.home_name_abbrev);
        obj1AwayIndex = FAVORITE_TEAMS.indexOf(obj1.away_name_abbrev);

        minOfObj1 = Math.min(
          numberOrMaxIfNegative(obj1HomeIndex),
          numberOrMaxIfNegative(obj1AwayIndex)
        );

        obj2HomeIndex = FAVORITE_TEAMS.indexOf(obj2.home_name_abbrev);
        obj2AwayIndex = FAVORITE_TEAMS.indexOf(obj2.away_name_abbrev);

        minOfObj2 = Math.min(
          numberOrMaxIfNegative(obj2HomeIndex),
          numberOrMaxIfNegative(obj2AwayIndex)
        );

        return minOfObj1 - minOfObj2;
      });

      extractStatsForGame(sortedGames[0]);
    } else {
      var randomGame =
        gamesWithFavoriteTeams[
          Math.floor(Math.random() * gamesWithFavoriteTeams.length)
        ];
      extractStatsForGame(randomGame);
    }
  });
}

function numberOrMaxIfNegative(number) {
  if (number < 0) {
    return 100;
  }
  return number;
}

// postToAPI sends data to the Particle API for your device.
//
//     postToAPI("lcdRow1", "Home Run!")
function postToAPI(apiName, data) {
  console.log("POSTING", apiName, data);
  var options = {
    method: "POST",
    url:
      "https://api.particle.io/v1/devices/" +
      process.env.PARTICLE_DEVICE_ID +
      "/" +
      apiName,
    headers: {
      Authorization: "Bearer " + process.env.PARTICLE_ACCESS_TOKEN
    },
    form: {
      arg: data
    }
  };

  console.log(options)

  function callback(error, response, body) {
    if (!error && response.statusCode == 200) {
      var info = JSON.parse(body);
    } else {
      console.log(error, response, body)
    }
  }

  request(options, callback);
}

function formatStats(stats) {
  // Send long string with game content for line 1 and line 2 of LCD.
  // EXAMPLE on two lines:
  // *NYY 10 Judg 1-2
  //  LAD 12    8  **
  // EXAMPLE on one line:
  // *NYY 10 Judg 1-2 LAD 12    8  **
  console.log("PBP:", stats.pbp);
  line1 = [
    (stats.top_inning == "Y" ? "*" : " ") +
      sprintf("%3s", stats.away_name_abbrev),
    sprintf("%2d", stats.away_name_runs),
    sprintf("%4s", stats.batter.substr(0, 4)),
    (stats.balls || 0) + "-" + (stats.strikes || 0)
  ].join(" ");
  line2 = [
    (stats.top_inning == "Y" ? " " : "*") +
      sprintf("%3s", stats.home_name_abbrev),
    sprintf("%2d", stats.home_name_runs),
    sprintf("%4d", stats.inning),
    sprintf("%3s", "*".repeat(stats.outs))
  ].join(" ");
  postToAPI("lcdRow1", line1 + line2);

  if (stats.runners_on_base != undefined && stats.runners_on_base.status > 0) {
    bases = [
      stats.runners_on_base.hasOwnProperty("runner_on_1b") ? "1" : "0",
      stats.runners_on_base.hasOwnProperty("runner_on_2b") ? "2" : "0",
      stats.runners_on_base.hasOwnProperty("runner_on_3b") ? "3" : "0"
    ].join("");
    // TODO: Should be %-16d
    // EXAMPLE: Runners on all bases (321)
    //          0000000000000321
    formattedBases = sprintf("%16s", bases);
    postToAPI("bases", formattedBases);
  } else {
    postToAPI("bases", "0000000000000000");
  }
}

function extractStatsForGame(element) {
  var stats = {
    away_name_abbrev: element.away_name_abbrev,
    home_name_abbrev: element.home_name_abbrev,
    balls: element.status.b,
    strikes: element.status.s,
    outs: element.status.o,
    top_inning: element.status.top_inning,
    inning: element.status.inning,
    away_name_runs: element.linescore.r.away,
    home_name_runs: element.linescore.r.home,
    runners_on_base: element.runners_on_base,
    batter: batterName(element),
    pbp: lastPlayByPlayEvent(element)
  };
  formatStats(stats);
}

function batterName(element) {
  if (element.batter != undefined) {
    return element.batter.name_display_roster;
  }
  return "";
}

function lastPlayByPlayEvent(element) {
  if (element.pbp != undefined) {
    return element.pbp.last;
  }
  return "";
}
