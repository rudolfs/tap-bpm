open Revery;
open Revery.Time;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Color = {
  let black = Color.rgb(0.0, 0.0, 0.0); /* text */

  let grey1 = Color.rgb(0.43, 0.43, 0.43); /* background */
  let grey2 = Color.rgb(0.59, 0.59, 0.59); /* foreground */
  let grey3 = Color.rgb(0.69, 0.69, 0.69); /* hover */
  let grey4 = Color.rgb(0.82, 0.82, 0.82); /* active */

  let yellow = Color.rgb(1.0, 0.71, 0.20); /* action */
};

module Typography = {
  let font = "Roboto-Regular.ttf";
  let size = 19;
  let lineHeight = 1.0;
};

module Button = {
  module Styles = {
    let clickable =
      Style.[
        position(`Relative),
        backgroundColor(Color.grey2),
        justifyContent(`Center),
        alignItems(`Center),
        width(52),
        height(34),
        marginRight(4),
      ];

    let container =
      Style.[
        position(`Relative),
        justifyContent(`Center),
        alignItems(`Center),
      ];

    let text =
      Style.[
        color(Colors.black),
        fontFamily(Typography.font),
        fontSize(Typography.size),
        lineHeight(Typography.lineHeight),
      ];
  };

  let component = React.component("Button");

  let createElement = (~text: string, ~onClick, ~children as _, ()) => {
    let content =
      <Clickable style=Styles.clickable onClick>
        <View style=Styles.container> <Text style=Styles.text text /> </View>
      </Clickable>;

    component(hooks => (hooks, content));
  };
};

module Display = {
  module Styles = {
    let container =
      Style.[
        height(34),
        backgroundColor(Color.grey2),
        justifyContent(`Center),
        alignItems(`Center),
        width(84),
      ];

    let text =
      Style.[
        color(Color.black),
        fontFamily(Typography.font),
        fontSize(Typography.size),
        lineHeight(Typography.lineHeight),
      ];
  };

  let component = React.component("Display");

  let createElement = (~children as _, ~value: float, ()) => {
    let content =
      <View style=Styles.container>
        <Text style=Styles.text text={string_of_float(value)} />
      </View>;

    component(hooks => (hooks, content));
  };
};

module Bpm = {
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

  let createElement = (~children as _, ()) =>
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

      let handleKeyPress = (_event: NodeEvents.keyEventParams) => {
        startStop();
        Console.log("handle");
      };

      let content =
        <View
          style=Style.[
            justifyContent(`Center),
            flexDirection(`Row),
            flexWrap(`Wrap),
            top(19),
          ]
          onKeyDown=handleKeyPress>
          <Button text="Tap" onClick=startStop />
          /* <Display value=122.22 /> */
          <Display value={state.elapsedTime |> Time.toSeconds} />
        </View>;

      (hooks, content);
    });
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=172,
          ~height=72,
          ~resizable=false,
          ~maximized=false,
          ~backgroundColor=Color.grey1,
          (),
        ),
      app,
      "bla",
    );

  let _ = UI.start(win, <Bpm />);

  ();
};

App.start(init);
