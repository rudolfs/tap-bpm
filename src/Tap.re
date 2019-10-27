open Revery;
open Revery.Time;
open Revery.UI;
open Revery.UI.Components;

type state =
  | Idle
  | Hover
  | Active;

type action =
  | Idle
  | Hover
  | Active;

module Styles = {
  let containerHover = Style.[backgroundColor(Theme.Color.grey3)];

  let containerActive = Style.[backgroundColor(Theme.Color.yellow)];

  let container =
    Style.[
      width(52),
      height(34),
      position(`Relative),
      justifyContent(`Center),
      alignItems(`Center),
      marginRight(4),
      backgroundColor(Theme.Color.grey2),
    ];

  let text =
    Style.[
      color(Theme.Color.black),
      fontFamily(Theme.Typo.font),
      fontSize(Theme.Typo.size),
      lineHeight(Theme.Typo.lineHeight),
    ];
};

let component = React.component("Tap");

let createElement = (~onClick, ~children as _, ()) =>
  component(hooks => {
    let (state, dispatch, hooks) =
      Hooks.reducer(
        ~initialState=Idle,
        (action, _state) =>
          switch (action) {
          | Idle => Idle
          | Hover => Hover
          | Active => Active
          },
        hooks,
      );

    let currentStyle =
      switch (state) {
      | Idle => Styles.container
      | Hover =>
        Style.merge(~source=Styles.container, ~target=Styles.containerHover)
      | Active =>
        Style.merge(~source=Styles.container, ~target=Styles.containerActive)
      };

    let content =
      <View
        style=currentStyle
        onMouseUp={_ => onClick()}
        onMouseOut={_ => dispatch(Idle)}
        onMouseOver={_ => dispatch(Hover)}
        onMouseDown={
          _ => {
            let prevState = state;
            dispatch(Active);
            let _ = Tick.timeout(_ => dispatch(prevState), Seconds(0.05));
            ();
          }
        }>
        <Text style=Styles.text text="Tap" />
      </View>;

    (hooks, content);
  });
