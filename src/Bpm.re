open Revery;
open Revery.Time;
open Revery.UI;

module Styles = {
  let container =
    Style.[
      justifyContent(`Center),
      flexDirection(`Row),
      flexWrap(`Wrap),
      top(19),
    ];
};

type dispose = unit => unit;
let noop = () => ();

type state = {
  dispose,
  isRunning: bool,
  elapsedTime: Time.t,
  bpm: float,
};

type action =
  | Start(dispose)
  | Stop
  | Tap(dispose)
  | TimerTick(Time.t);

let calculateBpm = (~delta, ~prevBpm) => {
  let elapsedSec = delta |> Time.toSeconds;
  let currentBpm = 60.0 /. elapsedSec;

  (prevBpm +. currentBpm) /. 2.0;
};

let reducer = (a, state) =>
  switch (a) {
  | Start(f) => {
      dispose: f,
      isRunning: true,
      elapsedTime: Seconds(0.),
      bpm: state.bpm,
    }
  | Stop =>
    state.dispose();
    let ret = {
      dispose: noop,
      isRunning: false,
      elapsedTime: Seconds(0.),
      bpm: calculateBpm(~delta=state.elapsedTime, ~prevBpm=state.bpm),
    };
    ret;
  | Tap(f) =>
    state.dispose();
    let ret = {
      dispose: f,
      isRunning: true,
      elapsedTime: Seconds(0.),
      bpm: calculateBpm(~delta=state.elapsedTime, ~prevBpm=state.bpm),
    };
    ret;
  | TimerTick(t) => {
      ...state,
      elapsedTime:
        state.isRunning
          ? Time.increment(state.elapsedTime, t) : state.elapsedTime,
    }
  };

let component = React.component("Bpm");

let createElement = (~children as _, ~quit, ()) =>
  component(hooks => {
    let (state, dispatch, hooks) =
      Hooks.reducer(
        ~initialState={
          isRunning: false,
          dispose: noop,
          elapsedTime: Seconds(0.),
          bpm: 0.0,
        },
        reducer,
        hooks,
      );

    let hooks =
      Hooks.effect(OnMount, () => Some(() => dispatch(Stop)), hooks);

    let startStop = () =>
      if (state.isRunning) {
        let dispose =
          Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));
        dispatch(Tap(dispose));
      } else {
        let dispose =
          Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));
        dispatch(Start(dispose));
      };

    let bpmString =
      if (state.bpm == 0.) {
        "BPM";
      } else {
        String.sub(string_of_float(state.bpm), 0, 6);
      };

    let content =
      <View style=Styles.container>
        <TapButton onClick=startStop />
        <TempoDisplay value=bpmString />
        <KeyboardInput tapCallback=startStop quit />
      </View>;

    (hooks, content);
  });
