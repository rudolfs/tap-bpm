open Revery;
open Revery.Time;
open Revery.UI;

module Styles = {
  let container = Style.[
    justifyContent(`Center),
    flexDirection(`Row),
    flexWrap(`Wrap),
    top(19),
  ];
}

type dispose = unit => unit;
let noop = () => ();

type state = {
  dispose,
  isRunning: bool,
  elapsedTime: Time.t,
};

type action =
  | Start(dispose)
  | Stop
  | TimerTick(Time.t);

let reducer = (a, s) =>
  switch (a) {
  | Start(f) => {dispose: f, isRunning: true, elapsedTime: Seconds(0.)}
  | Stop =>
    s.dispose();
    let ret = {dispose: noop, isRunning: false, elapsedTime: Seconds(0.)};
    ret;
  | TimerTick(t) => {
      ...s,
      elapsedTime:
        s.isRunning ? Time.increment(s.elapsedTime, t) : s.elapsedTime,
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
        },
        reducer,
        hooks,
      );

    let hooks =
      Hooks.effect(OnMount, () => Some(() => dispatch(Stop)), hooks);

    let startStop = () =>
      state.isRunning ?
        dispatch(Stop) :
        {
          let dispose =
            Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));

          dispatch(Start(dispose));
        };

    let content =
      <View style=Styles.container>
        <TapButton text="Tap" onClick=startStop />
        <TempoDisplay value={state.elapsedTime |> Time.toSeconds} />
        <KeyboardInput tapCallback=startStop quit/>
      </View>;

    (hooks, content);
  });
