angular.module("ruServer").directive("errorAlert", function () {
    return {
        templateUrl: "view/error.html",
        replace: true,
        restrict: "E",
        transclude: true
    };
});