const {
  requestBackend,
  getStoredToken,
  getStoredUser,
  clearAuth
} = require('../../utils/backend');

const COMMAND_LABELS = {
  key1: 'Light Toggle',
  key2: 'Fan Toggle',
  key3: 'Curtain Open',
  key4: 'Curtain Close',
  key5: 'Mode Switch'
};

Page({
  data: {
    uid: '25e95780fd1d407e86d6a685c8f7c3de',
    ledtopic: 'light002',
    dhttopic: 'zigbee',
    device_status: 'Offline',
    isLightOn: false,
    isFanOn: false,
    wendu: '',
    shidu: '',
    air: '',
    light: '',
    dataTime: '',
    userPhone: '',
    isAuthenticated: false
  },

  pollTimer: null,
  authRedirecting: false,

  onLoad() {
    this.bootstrap();
  },

  onShow() {
    if (this.data.isAuthenticated && !this.pollTimer) {
      this.startPolling();
    }
  },

  onHide() {
    this.stopPolling();
  },

  onUnload() {
    this.stopPolling();
  },

  bootstrap() {
    const cachedUser = getStoredUser();
    if (cachedUser && cachedUser.phone) {
      this.setData({ userPhone: cachedUser.phone });
    }

    this.ensureAuthenticated().then((ok) => {
      if (!ok) {
        return;
      }

      this.setData({ isAuthenticated: true });
      this.startPolling();
    });
  },

  ensureAuthenticated() {
    const token = getStoredToken();
    if (!token) {
      this.redirectToAuth(true);
      return Promise.resolve(false);
    }

    return requestBackend({
      url: '/api/auth/me',
      method: 'GET',
      auth: true
    })
      .then((user) => {
        if (user && user.phone) {
          wx.setStorageSync('user', user);
          this.setData({ userPhone: user.phone });
        }
        return true;
      })
      .catch(() => {
        this.redirectToAuth(true);
        return false;
      });
  },

  startPolling() {
    this.stopPolling();
    this.getOnline();
    this.getdht11();
    this.pollTimer = setInterval(() => {
      this.getdht11();
    }, 3000);
  },

  stopPolling() {
    if (this.pollTimer) {
      clearInterval(this.pollTimer);
      this.pollTimer = null;
    }
  },

  redirectToAuth(shouldClear = true) {
    if (this.authRedirecting) {
      return;
    }

    this.authRedirecting = true;
    this.stopPolling();

    if (shouldClear) {
      clearAuth();
      this.setData({
        isAuthenticated: false,
        userPhone: ''
      });
    }

    wx.reLaunch({
      url: '/pages/auth/index'
    });
  },

  logout() {
    clearAuth();
    wx.showToast({
      title: 'Logged out',
      icon: 'none'
    });
    this.redirectToAuth(false);
  },

  requireAuth() {
    if (!getStoredToken()) {
      this.redirectToAuth(true);
      return false;
    }
    return true;
  },

  handleBackendError(error) {
    if (error && error.statusCode === 401) {
      this.redirectToAuth(true);
    }
  },

  getOnline() {
    const that = this;
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/status/?',
      data: {
        uid: that.data.uid,
        topic: that.data.ledtopic
      },
      header: { 'content-type': 'application/x-www-form-urlencoded' },
      success(res) {
        if (res.data.status === 'online') {
          that.setData({ device_status: 'Online' });
        } else {
          that.setData({ device_status: 'Offline' });
        }
      }
    });
  },

  getdht11() {
    if (!this.requireAuth()) {
      return;
    }

    const that = this;
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/data/1/get/',
      data: {
        uid: that.data.uid,
        topic: that.data.dhttopic,
        num: 1
      },
      header: { 'content-type': 'application/x-www-form-urlencoded' },
      success(res) {
        const payload = String((res.data && res.data.msg) || '');
        const parts = payload.split('#');

        if (payload.indexOf('#') !== -1) {
          that.setData({
            wendu: parts[1] || '',
            shidu: parts[2] || '',
            air: parts[3] || '',
            light: parts[4] || '',
            dataTime: (res.data && res.data.time) || ''
          });
        }

        that.reportSensorLog({
          topic: that.data.dhttopic,
          temperature: parts[1] || '',
          humidity: parts[2] || '',
          air: parts[3] || '',
          brightness: parts[4] || '',
          bemfaTime: (res.data && res.data.time) || '',
          rawPayload: payload
        });
      }
    });
  },

  reportSensorLog(data) {
    if (!this.requireAuth()) {
      return;
    }

    requestBackend({
      url: '/api/logs/sensors',
      method: 'POST',
      auth: true,
      data
    }).catch((error) => {
      console.error('Sensor log upload failed:', error);
      this.handleBackendError(error);
    });
  },

  reportOperationLog(data) {
    if (!this.requireAuth()) {
      return;
    }

    requestBackend({
      url: '/api/logs/operations',
      method: 'POST',
      auth: true,
      data
    }).catch((error) => {
      console.error('Operation log upload failed:', error);
      this.handleBackendError(error);
    });
  },

  LedSendMsg(msg) {
    if (!this.requireAuth()) {
      return;
    }

    const that = this;
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/data/1/push/get/',
      data: {
        uid: that.data.uid,
        topic: that.data.ledtopic,
        msg
      },
      header: { 'content-type': 'application/x-www-form-urlencoded' },
      success(res) {
        const status = String((res.data && res.data.status) || '').toLowerCase();
        const isOk = status.includes('ok');

        if (isOk) {
          let toastTitle = 'Operation successful';

          if (msg === 'key1') {
            const nextLightState = !that.data.isLightOn;
            that.setData({ isLightOn: nextLightState });
            toastTitle = nextLightState ? 'Light turned on' : 'Light turned off';
          } else if (msg === 'key2') {
            const nextFanState = !that.data.isFanOn;
            that.setData({ isFanOn: nextFanState });
            toastTitle = nextFanState ? 'Fan turned on' : 'Fan turned off';
          } else if (msg === 'key3') {
            toastTitle = 'Curtain opened';
          } else if (msg === 'key4') {
            toastTitle = 'Curtain closed';
          } else if (msg === 'key5') {
            toastTitle = 'Mode switched successfully';
          }

          wx.showToast({
            title: toastTitle,
            icon: 'none'
          });
        } else {
          wx.showToast({
            title: 'Operation failed',
            icon: 'none'
          });
        }

        that.reportOperationLog({
          topic: that.data.ledtopic,
          commandKey: msg,
          commandLabel: COMMAND_LABELS[msg] || msg,
          result: isOk ? 'SUCCESS' : 'FAIL',
          bemfaResponse: JSON.stringify(res.data || {})
        });
      },
      fail(err) {
        wx.showToast({
          title: 'Network failed',
          icon: 'none'
        });

        that.reportOperationLog({
          topic: that.data.ledtopic,
          commandKey: msg,
          commandLabel: COMMAND_LABELS[msg] || msg,
          result: 'FAIL',
          bemfaResponse: JSON.stringify(err || {})
        });
      }
    });
  },

  sendKey(e) {
    const key = e.currentTarget.dataset.key;
    this.LedSendMsg(key);
  }
});
