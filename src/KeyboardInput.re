open Revery;
open Revery.UI;

type state = {
  ref: option(node),
  hasFocus: bool,
};

type action =
  | Focused(bool)
  | SetRef(node);

let reducer = (action, state) =>
  switch (action) {
  | Focused(v) => {...state, hasFocus: v}
  | SetRef(v) => {...state, ref: Some(v)}
  };
let component = React.component("KeyboardInput");

let createElement = (~children as _, ~onKeyDown, ()) =>
  component(hooks => {
    let (v, dispatch, hooks) =
      Hooks.reducer(
        ~initialState={ref: None, hasFocus: false},
        reducer,
        hooks,
      );

    let hooks =
      Hooks.effect(
        Always,
        () => {
          if (!v.hasFocus) {
            switch (v.ref) {
            | Some(v) => Focus.focus(v)
            | None => ()
            };
          };
          None;
        },
        hooks,
      );

    let onBlur = () => dispatch(Focused(false));

    let onFocus = () => dispatch(Focused(true));
    (
      hooks,
      <View
        ref={r => dispatch(SetRef(r))}
        onBlur
        onFocus
        onKeyDown
        style=Style.[position(`Absolute), width(1), height(1)]
      />,
    );
  });
