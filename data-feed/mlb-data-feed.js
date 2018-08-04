
const sprintf = require('sprintf-js').sprintf;

const Mlbgames = require('mlbgames');
const options = {
  path: 'year_2018/month_08/day_04/'
};

const mlbgames = new Mlbgames(options);
mlbgames.get((err, games) => {
  gameForTeam(games, "NYY")
});

function formatStats(stats) {
  line1 = [
    (stats.top_inning == "Y" ? "*" : " ") + stats.away_name_abbrev,
    sprintf("%2d", stats.away_name_runs),
    "    ",
    stats.balls + "-" + stats.strikes
  ].join(" ")
  line2 = [
    (stats.top_inning == "Y" ? " " : "*") + stats.home_name_abbrev,
    sprintf("%2d", stats.home_name_runs),
    sprintf("%4d", stats.inning),
    sprintf("%3s", "*".repeat(stats.outs))
  ].join(" ")
  console.log(line1 + line2)
}

function gameForTeam(games, teamName) {
  games.forEach(function(element) {
    if (element.away_name_abbrev == teamName || element.home_name_abbrev == teamName) {
      var stats = {
        "away_name_abbrev": element.away_name_abbrev,
        "home_name_abbrev": element.home_name_abbrev,
        "balls": element.status.b,
        "strikes": element.status.s,
        "outs": element.status.o,
        "top_inning": element.status.top_inning,
        "inning": element.status.inning,
        "away_name_runs": element.linescore.r.away,
        "home_name_runs": element.linescore.r.home
      }
      formatStats(stats)
    }
  });
}


