angular.module("ruServer").factory("timestampInterceptor", function () {
    return {
        request: function (config) {
            var _url = config.url;
            if (_url.indexOf('view') > -1) return config; 
            var _timestamp = new Date().getTime();
            config.url = _url + "?timestamp=" + _timestamp;
            return config;
        }
    };
});