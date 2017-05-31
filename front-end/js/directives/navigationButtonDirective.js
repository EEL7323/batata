angular.module("ruServer").directive("navigationMenu", function () {
    return {
        controller: function ($scope, $element, $attrs) {

            var buttons = [];

            this.registerButton = function (button) {
                buttons.push(button);
            };

            this.turnAllOff = function () {
                buttons.forEach(function (button) {
                    button.color = "btn-primary";
                });
            };
        }
    };

});

angular.module("ruServer").directive("navigationButton", function () {
    return {
        templateUrl: "view/nav-button.html",
        scope: {
            title: "@",
            page: "@"
        },
        require: "^navigationMenu",
        link: function (scope, element, attrs, ctrl) {
            ctrl.registerButton(scope);
            ctrl.turnAllOff();
            scope.selectButton = function () {
                ctrl.turnAllOff();
                scope.color = "btn-info";
            };
        }
    };

});