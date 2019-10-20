open Revery.UI;
open Revery.UI.Components;

module Styles = {
  let container =
    Style.[
      height(34),
      backgroundColor(Theme.Color.grey2),
      justifyContent(`Center),
      alignItems(`Center),
      width(84),
    ];

  let text =
    Style.[
      color(Theme.Color.black),
      fontFamily(Theme.Typo.font),
      fontSize(Theme.Typo.size),
      lineHeight(Theme.Typo.lineHeight),
    ];
};

let component = React.component("Display");

let createElement = (~children as _, ~value: string, ()) => {
  let content =
    <View style=Styles.container>
      <Text style=Styles.text text=value />
    </View>;

  component(hooks => (hooks, content));
};
