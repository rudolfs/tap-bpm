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
  bpm: string,
};

type action =
  | Start(dispose)
  | Stop
  | TimerTick(Time.t);

let bpm = delta => String.sub(string_of_float(60.0 /. delta), 0, 6);

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
      bpm: bpm(state.elapsedTime |> Time.toSeconds),
    };
    ret;
  | TimerTick(t) => {
      ...state,
      elapsedTime:
        state.isRunning ?
          Time.increment(state.elapsedTime, t) : state.elapsedTime,
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
          bpm: "BPM",
        },
        reducer,
        hooks,
      );

    let hooks =
      Hooks.effect(OnMount, () => Some(() => dispatch(Stop)), hooks);

    let startStop = () =>
      if (state.isRunning) {
        dispatch(Stop);
      } else {
        let dispose =
          Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));
        dispatch(Start(dispose));
      };

    let content =
      <View style=Styles.container>
        <TapButton text="Tap" onClick=startStop />
        /* <TempoDisplay value={state.elapsedTime |> Time.toSeconds} /> */
        <TempoDisplay value={state.bpm} />
        <KeyboardInput tapCallback=startStop quit />
      </View>;

    (hooks, content);
  });
