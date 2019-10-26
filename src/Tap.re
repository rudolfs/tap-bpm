open Revery;
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
  let clickable =
    Style.[
      position(`Relative),
      backgroundColor(Theme.Color.grey2),
      justifyContent(`Center),
      alignItems(`Center),
      width(52),
      height(34),
      marginRight(4),
    ];

  let clickableHover =
    Style.[
      backgroundColor(Theme.Color.grey3),
    ];

  let clickableActive =
    Style.[
      backgroundColor(Theme.Color.yellow),
    ];

  let container =
    Style.[
      position(`Relative),
      justifyContent(`Center),
      alignItems(`Center),
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

let createElement = (~onClick, ~children as _, ()) => {
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
    | Idle => Styles.clickable
    | Hover => Style.merge(~source=Styles.clickable, ~target=Styles.clickableHover)
    | Active => Style.merge(~source=Styles.clickable, ~target=Styles.clickableActive)
    };

  let content =
    <Clickable
      style=currentStyle onClick>
      <View
        onMouseOut={_ => dispatch(Idle)}
        onMouseOver={_ => dispatch(Hover)}
        onMouseDown={_ => dispatch(Active)}
        style=Styles.container>
        <Text style=Styles.text text="Tap" />
      </View>
    </Clickable>;

    (hooks, content);
  });
};
