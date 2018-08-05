const sprintf = require("sprintf-js").sprintf;
const Mlbgames = require("mlbgames");
const request = require("request");

const FAVORITE_TEAM = "HOU"; // NYY CWS STL NYM

const options = {
  path: "year_2018/month_08/day_04/"
};

const mlbgames = new Mlbgames(options);
mlbgames.get((err, games) => {
  gameForTeam(games, FAVORITE_TEAM);
});

// postToAPI sends data to the Particle API for your device.
//
//     postToAPI("lcdRow1", "Home Run!")
function postToAPI(apiName, data) {
  console.log("POSTING", apiName, data)
  var options = {
    method: "POST",
    url: "https://api.particle.io/v1/devices/" + process.env.SPARK_DEVICE_ID + "/" + apiName,
    headers: {
      "Authorization": "Bearer " + process.env.SPARK_TOKEN
    },
    form: {
      arg: data
    }
  };

  function callback(error, response, body) {
    if (!error && response.statusCode == 200) {
      var info = JSON.parse(body);
    }
  }

  request(options, callback);
}

function formatStats(stats) {
  line1 = [
    (stats.top_inning == "Y" ? "*" : " ") +
      sprintf("%3s", stats.away_name_abbrev),
    sprintf("%2d", stats.away_name_runs),
    "    ",
    stats.balls + "-" + stats.strikes
  ].join(" ");
  line2 = [
    (stats.top_inning == "Y" ? " " : "*") +
      sprintf("%3s", stats.home_name_abbrev),
    sprintf("%2d", stats.home_name_runs),
    sprintf("%4d", stats.inning),
    sprintf("%3s", "*".repeat(stats.outs))
  ].join(" ");
  postToAPI("lcdRow1", (line1 + line2))

  if (stats.runners_on_base.status > 0) {
    bases = [
      stats.runners_on_base.hasOwnProperty("runner_on_1b") ? "1" : "0",
      stats.runners_on_base.hasOwnProperty("runner_on_2b") ? "2" : "0",
      stats.runners_on_base.hasOwnProperty("runner_on_3b") ? "3" : "0"
    ].join("");
    formattedBases = sprintf("%16s", bases);
    postToAPI("bases", formattedBases)
  } else {
    postToAPI("bases", "0000000000000000");
  }
}

function gameForTeam(games, teamName) {
  games.forEach(function(element) {
    if (
      element.away_name_abbrev == teamName ||
      element.home_name_abbrev == teamName
    ) {
      //  runners_on_base: { status: '0' }
      // console.log(element.runners_on_base)
      // runners_on_base:
      // { runner_on_1b:
      // { first: 'Matt',
      //   id: '622110',
      //   last: 'Duffy',
      //   name_display_roster: 'Duffy',
      //   number: '5' },
      // status: '1' } }
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
        runners_on_base: element.runners_on_base
      };
      formatStats(stats);
    }
  });
}
